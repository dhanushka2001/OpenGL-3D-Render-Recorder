# If you get a "Re-checking globbed directories... Re-running CMake..." infinite loop when running "ninja", press Ctrl+C and just delete CMakeCache.txt in /build/, then rerun "ninja".

# Minimum CMake version
cmake_minimum_required(VERSION 3.31)

# Project name and version
project(OpenGL-3D-Render-Recorder VERSION 1.2.0)

# for LazyVim
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set C++ standard
set(CMAKE_CXX_STANDARD 		 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS	 ON)

# CMake Build Type (Debug/Release)
set(CMAKE_BUILD_TYPE Release)

# set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

# Specify the source files
set(SOURCES
    src/main.cpp
    src/third_party/glad.c
    src/ultralight_browser.cpp
    #src/cef_integration.cpp
    src/input_callbacks.cpp
    src/third_party/miniaudio_impl.cpp
)

# Include headers for FFmpeg and others
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/freetype2
    ${CMAKE_SOURCE_DIR}/include/ultralight
    # ${CMAKE_SOURCE_DIR}/lib/cef/include
    # ${CMAKE_SOURCE_DIR}/lib/cef/libcef_dll
)
if(WIN32)
include_directories(
   ${CMAKE_SOURCE_DIR}/include/ffmpeg_windows
)
endif(WIN32)

# Executable target
add_executable(${PROJECT_NAME} ${SOURCES})

# Define the source and destination directories for /assets/
set(ASSETS_SOURCE_DIR ${CMAKE_SOURCE_DIR}/assets)
set(ASSETS_DEST_DIR   ${CMAKE_BINARY_DIR})

# Define the source and destination directories for /shaders/
set(SHADERS_SOURCE_DIR ${CMAKE_SOURCE_DIR}/shaders)
set(SHADERS_DEST_DIR   ${CMAKE_BINARY_DIR})

# Shader copy target
if(UNIX)
    find_program(RSYNC_EXE rsync) # Removed REQUIRED to handle the error ourselves

    if(NOT RSYNC_EXE)
        message(FATAL_ERROR "rsync not found. Please install it (e.g. 'sudo pacman -S rsync') to build this project.")
    endif()

    add_custom_target(sync_assets ALL
        COMMAND ${RSYNC_EXE} -au ${ASSETS_SOURCE_DIR} ${ASSETS_DEST_DIR}
        COMMAND ${RSYNC_EXE} -au ${SHADERS_SOURCE_DIR} ${SHADERS_DEST_DIR}
        COMMENT "Syncing assets and shaders using rsync"
    )
    
    # ULTRALIGHT
    set(ULTRALIGHT_LIB_DIR ${CMAKE_SOURCE_DIR}/lib/ultralight/linux/bin)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/lib/ultralight/linux/bin
        ${CMAKE_BINARY_DIR}/bin
    )
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/lib/ultralight/linux/resources
        ${CMAKE_BINARY_DIR}/bin/resources
    )

    # CEF
    # ---
    set(CEF_VERSION "146.0.6+g68649e2+chromium-146.0.7680.154" CACHE STRING "CEF version")
    set(CEF_ROOT ${CMAKE_SOURCE_DIR}/lib/cef)
    set(CEF_DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/cef_download)
    set(CEF_ARCHIVE ${CMAKE_BINARY_DIR}/cef.tar.bz2)
    set(CEF_EXTRACTED_DIR ${CEF_DOWNLOAD_DIR}/extracted)


    set(CEF_URL "https://cef-builds.spotifycdn.com/cef_binary_${CEF_VERSION}_linux64.tar.bz2")
    
    # -----------------------------------
    # DOWNLOAD + EXTRACT (only if needed)
    # -----------------------------------
    if(NOT EXISTS ${CEF_ROOT}/Release/libcef.so)
        message(STATUS "CEF NOT found. Downloading CEF...")
    
        file(MAKE_DIRECTORY ${CEF_DOWNLOAD_DIR})
        
        file(DOWNLOAD
            ${CEF_URL}
            ${CEF_ARCHIVE}
            SHOW_PROGRESS
        )
    
        message(STATUS "Extracting CEF...")
    
        file(ARCHIVE_EXTRACT
            INPUT ${CEF_ARCHIVE}
            DESTINATION ${CEF_EXTRACTED_DIR}
        )
    
        # CEF extracts into a versioned folder
        file(GLOB CEF_EXTRACTED_SUBDIR
            ${CEF_EXTRACTED_DIR}/cef_binary_*
        )

        list(GET CEF_EXTRACTED_SUBDIR 0 CEF_SRC_DIR)

        message(STATUS "Installing CEF to ${CEF_ROOT}...")

        file(MAKE_DIRECTORY ${CEF_ROOT})

        file(COPY
            ${CEF_SRC_DIR}/include
            ${CEF_SRC_DIR}/Release
            ${CEF_SRC_DIR}/Resources
            ${CEF_SRC_DIR}/libcef_dll
            DESTINATION ${CEF_ROOT}
        )
    else()
        message(STATUS "CEF found: ${CEF_ROOT}/Release/libcef.so")
    endif()

    # -------------------------
    # COPY lib/cef TO build/bin
    # -------------------------
    set(CEF_LIB_DIR ${CEF_ROOT}/Release)
    
    # Copy ALL runtime files (Release + Resources)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CEF_ROOT}/Release
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
    
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CEF_ROOT}/Resources
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
    
    # -----------------------------------------------------------
    # CEF WRAPPER (recurse into ALL subdirectories of libcef_dll)
    # -----------------------------------------------------------
    file(GLOB_RECURSE CEF_WRAPPER_SOURCES CONFIGURE_DEPENDS
        ${CEF_ROOT}/libcef_dll/*.cc
    )
    
    add_library(cef_dll_wrapper STATIC ${CEF_WRAPPER_SOURCES})
    
    target_include_directories(cef_dll_wrapper PUBLIC
        ${CEF_ROOT}
        ${CEF_ROOT}/include
    )
    
    target_compile_definitions(cef_dll_wrapper PUBLIC
        LINUX
        CEF_USE_SANDBOX=0
        USING_CEF_SHARED
        WRAPPING_CEF_SHARED
    )

    # ----------------
    # IMPORT libcef.so
    # ----------------
    add_library(libcef SHARED IMPORTED)
    set_target_properties(libcef PROPERTIES
        IMPORTED_LOCATION "${CEF_LIB_DIR}/libcef.so"
        INTERFACE_INCLUDE_DIRECTORIES "${CEF_ROOT}/include"
    )

    # --------------------------------------------------
    # Tell the linker where to find libcef.so at runtime
    # --------------------------------------------------
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath,'$ORIGIN'")
endif(UNIX)

if(WIN32)
    # Set source and destination directories
    set(ASSETS_SOURCE_DIR   ${CMAKE_SOURCE_DIR}/assets)
    set(ASSETS_DEST_DIR     ${CMAKE_BINARY_DIR}/assets)
    set(SHADERS_SOURCE_DIR  ${CMAKE_SOURCE_DIR}/shaders)
    set(SHADERS_DEST_DIR    ${CMAKE_BINARY_DIR}/shaders)

    # Convert paths to Windows native format
    file(TO_NATIVE_PATH ${ASSETS_SOURCE_DIR}    ASSETS_SRC_NATIVE)
    file(TO_NATIVE_PATH ${ASSETS_DEST_DIR}      ASSETS_DST_NATIVE)
    file(TO_NATIVE_PATH ${SHADERS_SOURCE_DIR}   SHADERS_SRC_NATIVE)
    file(TO_NATIVE_PATH ${SHADERS_DEST_DIR}     SHADERS_DST_NATIVE)

    # Recursively find all files to track for rebuilds
    file(GLOB_RECURSE ASSET_FILES CONFIGURE_DEPENDS RELATIVE ${ASSETS_SOURCE_DIR} "${ASSETS_SOURCE_DIR}/*")
    file(GLOB_RECURSE SHADER_FILES CONFIGURE_DEPENDS RELATIVE ${SHADERS_SOURCE_DIR} "${SHADERS_SOURCE_DIR}/*")

    # Prepend full path to each file
    list(TRANSFORM ASSET_FILES PREPEND "${ASSETS_SOURCE_DIR}/")
    list(TRANSFORM SHADER_FILES PREPEND "${SHADERS_SOURCE_DIR}/")

    # Create stamp file names
    set(ASSETS_COPY_STAMP ${CMAKE_BINARY_DIR}/assets_robocopy.stamp)
    set(SHADERS_COPY_STAMP ${CMAKE_BINARY_DIR}/shaders_robocopy.stamp)

    # Robocopy for assets
    add_custom_command(
        OUTPUT ${ASSETS_COPY_STAMP}
        COMMAND cmd /c robocopy "${ASSETS_SRC_NATIVE}" "${ASSETS_DST_NATIVE}" /MIR /NJH /NJS /NFL /NDL /NS /NC /NP || exit 0
        COMMAND ${CMAKE_COMMAND} -E touch "${ASSETS_COPY_STAMP}"
        DEPENDS ${ASSET_FILES}
        COMMENT "Syncing /assets/ with robocopy"
        VERBATIM
    )

    # Robocopy for shaders
    add_custom_command(
        OUTPUT ${SHADERS_COPY_STAMP}
        COMMAND cmd /c robocopy "${SHADERS_SRC_NATIVE}" "${SHADERS_DST_NATIVE}" /MIR /NJH /NJS /NFL /NDL /NS /NC /NP || exit 0
        COMMAND ${CMAKE_COMMAND} -E touch "${SHADERS_COPY_STAMP}"
        DEPENDS ${SHADER_FILES}
        COMMENT "Syncing /shaders/ with robocopy"
        VERBATIM
    )

    # Target that triggers both syncs
    add_custom_target(sync_resources ALL
        DEPENDS ${ASSETS_COPY_STAMP} ${SHADERS_COPY_STAMP}
    )

    # Copy Ultralight DLLs
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/lib/ultralight/windows/bin
        ${CMAKE_BINARY_DIR}/bin
    )
    
    # Copy Ultralight resources
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/lib/ultralight/windows/resources
        ${CMAKE_BINARY_DIR}/bin/resources
    )

    # # Always-run resource copy target
    # add_custom_target(sync_resources ALL
    #     COMMAND cmd /c robocopy "${ASSETS_SRC_NATIVE}" "${ASSETS_DST_NATIVE}" /MIR /NJH /NJS /NFL /NDL /NS /NC /NP || exit 0
    #     COMMAND cmd /c robocopy "${SHADERS_SRC_NATIVE}" "${SHADERS_DST_NATIVE}" /MIR /NJH /NJS /NFL /NDL /NS /NC /NP || exit 0
    #     COMMENT "Syncing assets/ and shaders/ with robocopy"
    #     VERBATIM
    # )
endif(WIN32)



# # Add the browser function library
# add_library(browser STATIC
#     src/browser.cpp
# )
# # Set the target properties for browser (where libbrowser.a will be created)
# set_target_properties(browser PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/learnopengl)

# Add the custom timer function library
add_library(timer STATIC
    src/timer.cpp
)
# Set the target properties for text (where libtimer.a will be created)
set_target_properties(timer PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/learnopengl)

# Add the custom text function library
add_library(text STATIC
    src/font.cpp
    src/fontmanager.cpp
    src/textrenderer.cpp
)
# Set the target properties for text (where libtext.a will be created)
set_target_properties(text PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/learnopengl)

# Add the FFmpeg function library
add_library(encoder STATIC
    src/encoder.cpp
)
# Set the target properties for encoder (where libencoder.a will be created)
set_target_properties(encoder PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/learnopengl)

# Add the GUI library that handles ImGui/ImPlot
add_library(gui STATIC
    src/gui.cpp
)
# Set the target properties for encoder (where libgui.a will be created)
set_target_properties(gui PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/learnopengl)

# Add the ImGui library
add_library(imgui STATIC
    src/third_party/imgui/imgui.cpp 
    src/third_party/imgui/imgui_draw.cpp 
    src/third_party/imgui/imgui_tables.cpp 
    src/third_party/imgui/imgui_widgets.cpp
    src/third_party/imgui/imgui_impl_opengl3.cpp
    src/third_party/imgui/imgui_impl_glfw.cpp
    src/third_party/imgui/imgui_demo.cpp
)
# Set the target properties for imgui (where libimgui.a will be created)
set_target_properties(imgui PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/imgui)

# Add the ImPlot library
add_library(implot STATIC 
    src/third_party/implot/implot.cpp
    src/third_party/implot/implot_items.cpp
    src/third_party/implot/implot_demo.cpp
)
# Set the target properties for implot (where libimplot.a will be created)
set_target_properties(implot PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/implot)

# Include headers
target_include_directories(imgui PUBLIC 
    ${CMAKE_SOURCE_DIR}/include/imgui
)
target_include_directories(implot PUBLIC
    ${CMAKE_SOURCE_DIR}/include/implot
)

# Link ImPlot to ImGui
target_link_libraries(implot PUBLIC imgui)  # ImPlot depends on ImGui

# Add the Settings library
add_library(settings STATIC
    src/Settings.cpp
)

# Set the target properties for settings (where libsettings.a will be created)
set_target_properties(settings PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/learnopengl)

# Link settings into any target that needs it
target_link_libraries(encoder PUBLIC settings)
target_link_libraries(text PUBLIC settings)
target_link_libraries(gui PUBLIC settings)

# Link gui to imgui (which implot is also linked to)
target_link_libraries(gui PUBLIC imgui)
target_link_libraries(gui PUBLIC implot)

if(UNIX)
    find_package(OpenGL REQUIRED)
    find_package(glfw3 QUIET)
    if(NOT glfw3_FOUND)
        include(FetchContent)
        FetchContent_Declare(
            glfw
            GIT_REPOSITORY https://github.com/glfw/glfw
            GIT_TAG 3.4
        )
        FetchContent_MakeAvailable(glfw)
    endif()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET
        libavcodec
        libavformat
        libavutil
        libswscale
    )
    pkg_check_modules(X264 REQUIRED IMPORTED_TARGET
        x264
    )
    pkg_check_modules(FREETYPE2 REQUIRED IMPORTED_TARGET
        freetype2
    )
    pkg_check_modules(GTK3 REQUIRED IMPORTED_TARGET
        gtk+-3.0
    )
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${X264_INCLUDE_DIRS}
        ${FFMPEG_INCLUDE_DIRS}
        ${FREETYPE2_INCLUDE_DIRS}
        ${GTK3_INCLUDE_DIRS}
    )
    message(STATUS "FFMPEG include dirs: ${FFMPEG_INCLUDE_DIRS}")
    message(STATUS "X264 include dirs: ${X264_INCLUDE_DIRS}")
    message(STATUS "FREETYPE2 include dirs: ${FREETYPE2_INCLUDE_DIRS}")
    target_link_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/ultralight/linux/lib
    )
    target_link_libraries(${PROJECT_NAME}
	    # browser
        timer
        gui      # gui depends on imgui and implot
        implot   # ImPlot static library from CMake
        imgui    # ImGui static library from CMake (don't even need to link as ImPlot already linked to ImGui, but no harm in including again)
        encoder
        text
        settings # don't even need to link as settings already linked to encoder and text, but no harm in including again
        OpenGL::GL
        glfw
        PkgConfig::FFMPEG
        PkgConfig::X264
        PkgConfig::FREETYPE2
    
        # Ultralight shared libs — use full paths
        ${ULTRALIGHT_LIB_DIR}/libAppCore.so
        ${ULTRALIGHT_LIB_DIR}/libWebCore.so
        ${ULTRALIGHT_LIB_DIR}/libUltralightCore.so
        ${ULTRALIGHT_LIB_DIR}/libUltralight.so

        # GTK
        ${GTK3_LIBRARIES}
        
        # CEF
        cef_dll_wrapper
        libcef
        
        # Linux OpenGL / system libs
        X11 pthread dl m
    )
endif(UNIX)

if(WIN32)
    target_link_libraries(${PROJECT_NAME}
        settings # don't even need to link as settings already linked to encoder and text, but no harm in including again
        imgui    # ImGui static library from CMake (don't even need to link as ImPlot already linked to ImGui, but no harm in including again)
        implot   # ImPlot static library from CMake
        encoder
        text
        timer
        gui
	browser

        ${CMAKE_SOURCE_DIR}/lib/GLFW/libglfw3.a
        ${CMAKE_SOURCE_DIR}/lib/OpenGL/libopengl32.a  # OpenGL

        # FFmpeg
        ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libavformat.a
        ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libavcodec.a
        ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libswscale.a     # must link before libavutil.a (swscale depends on avutil)
        ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libavutil.a
        ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libstrmiids.a
        ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libws2_32.a
        ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libbcrypt.a
        
        # x264
        ${CMAKE_SOURCE_DIR}/lib/x264/libx264.a

        # Freetype
        ${CMAKE_SOURCE_DIR}/lib/freetype/libfreetype.a

	# Ultralight
	${CMAKE_SOURCE_DIR}/lib/ultralight/windows/lib/Ultralight.lib
	${CMAKE_SOURCE_DIR}/lib/ultralight/windows/lib/WebCore.lib
	${CMAKE_SOURCE_DIR}/lib/ultralight/windows/lib/AppCore.lib
    )
endif(WIN32)

# Set output directory for the executable
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Add a custom 'clean' target
add_custom_target(clean-all
   COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/*
   COMMENT "Cleaning up the build directory"
)

# Delete the CMakeCache.txt file (fixes "Rechecking globbed directories..." infinite loop issue when running "ninja" sometimes)
# Run inside build/: cmake --build . --target clear
add_custom_target(clear
    COMMAND ${CMAKE_COMMAND} -E rm -f "${CMAKE_BINARY_DIR}/CMakeCache.txt"
    COMMENT "Clearing CMake cache..."
    VERBATIM
)

# Run inside build/: ninja zip OR cmake --build . --target zip
set(PACKAGE_NAME "OpenGL-3D-Render-Recorder-v1.2.0-Windows-x86_64")
# set(PACKAGE_NAME "OpenGL-3D-Render-Recorder-v1.2.0-Arch-Linux-x86_64")
set(PACKAGE_DIR "${CMAKE_BINARY_DIR}/${PACKAGE_NAME}")
set(PACKAGE_ZIP "${CMAKE_BINARY_DIR}/${PACKAGE_NAME}.zip")
# Uncomment ALL to make CMake run this target by default (always makes ZIP file)
add_custom_target(zip #ALL
    # Clean previous packaging folder if it exists
    COMMAND ${CMAKE_COMMAND} -E rm -rf "${PACKAGE_DIR}"

    # Create directory structure
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PACKAGE_DIR}/bin"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PACKAGE_DIR}/shaders"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PACKAGE_DIR}/assets"

    # Copy binaries except imgui.ini
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_BINARY_DIR}/bin" "${PACKAGE_DIR}/bin"
    COMMAND ${CMAKE_COMMAND} -E rm -f "${PACKAGE_DIR}/bin/imgui.ini"

    # Copy shaders and assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/shaders" "${PACKAGE_DIR}/shaders"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/assets" "${PACKAGE_DIR}/assets"

    # Create the ZIP archive (bin, shaders, assets under PACKAGE_NAME/)
    COMMAND ${CMAKE_COMMAND} -E tar "cfv" "${PACKAGE_ZIP}" --format=zip -- "${PACKAGE_NAME}"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"

    COMMENT "Packaging into ${PACKAGE_NAME}.zip with top-level folder"

    # Delete unzipped packaging folder
    COMMAND ${CMAKE_COMMAND} -E rm -rf "${PACKAGE_DIR}"

    VERBATIM
)


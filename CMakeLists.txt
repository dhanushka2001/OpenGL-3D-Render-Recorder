# If you get a "Re-checking globbed directories... Re-running CMake..." infinite loop when running "ninja", press Ctrl+C and just delete CMakeCache.txt in /build/, then rerun "ninja".

# Minimum CMake version
cmake_minimum_required(VERSION 3.31)

# Project name and version
project(myprogram VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 		 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS	 ON)

# CMake Build Type (Debug/Release)
set(CMAKE_BUILD_TYPE Release)

# Specify the source files
set(SOURCES
    src/main.cpp
    src/third_party/glad.c
)

# Include headers for FFmpeg and others
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/freetype2
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
    add_custom_target(sync_assets ALL
        COMMAND rsync -a --delete ${ASSETS_SOURCE_DIR} ${ASSETS_DEST_DIR}
        COMMAND rsync -a --delete ${SHADERS_SOURCE_DIR} ${SHADERS_DEST_DIR}
        COMMENT "Syncing assets and shaders using rsync"
    )
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

    # # Always-run resource copy target
    # add_custom_target(sync_resources ALL
    #     COMMAND cmd /c robocopy "${ASSETS_SRC_NATIVE}" "${ASSETS_DST_NATIVE}" /MIR /NJH /NJS /NFL /NDL /NS /NC /NP || exit 0
    #     COMMAND cmd /c robocopy "${SHADERS_SRC_NATIVE}" "${SHADERS_DST_NATIVE}" /MIR /NJH /NJS /NFL /NDL /NS /NC /NP || exit 0
    #     COMMENT "Syncing assets/ and shaders/ with robocopy"
    #     VERBATIM
    # )
endif(WIN32)

# Add the custom timer function library
add_library(timer STATIC
    src/timer.cpp
)
# Set the target properties for text (where libtext.a will be created)
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
# Set the target properties for encoder (where libencoder.a will be created)
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
    find_package(glfw3 REQUIRED)
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
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${X264_INCLUDE_DIRS}
        ${FFMPEG_INCLUDE_DIRS}
        ${FREETYPE2_INCLUDE_DIRS}
    )
    message(STATUS "FFMPEG include dirs: ${FFMPEG_INCLUDE_DIRS}")
    message(STATUS "X264 include dirs: ${X264_INCLUDE_DIRS}")
    target_link_libraries(${PROJECT_NAME}
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
set(PACKAGE_NAME "MyProgram-v1.0.0")
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


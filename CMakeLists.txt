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
    src/glad.c
)

# Include headers for FFmpeg and others
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/freetype2
)

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
    set(ASSETS_SOURCE_DIR ${CMAKE_SOURCE_DIR}/assets)
    set(SHADERS_SOURCE_DIR ${CMAKE_SOURCE_DIR}/shaders)
    set(ASSETS_DEST_DIR ${CMAKE_BINARY_DIR}/assets)
    set(SHADERS_DEST_DIR ${CMAKE_BINARY_DIR}/shaders)

    # Convert paths to Windows native format
    file(TO_NATIVE_PATH ${ASSETS_SOURCE_DIR} ASSETS_SRC_NATIVE)
    file(TO_NATIVE_PATH ${ASSETS_DEST_DIR} ASSETS_DST_NATIVE)
    file(TO_NATIVE_PATH ${SHADERS_SOURCE_DIR} SHADERS_SRC_NATIVE)
    file(TO_NATIVE_PATH ${SHADERS_DEST_DIR} SHADERS_DST_NATIVE)

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
endif(WIN32)

# Add the custom timer function library
add_library(timer STATIC
    src/timer.cpp
)
# Set the target properties for text (where libtext.a will be created)
set_target_properties(timer PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/)

# Add the custom text function library
add_library(text STATIC
    src/font.cpp
    src/fontmanager.cpp
    src/textrenderer.cpp
)
# Set the target properties for text (where libtext.a will be created)
set_target_properties(text PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/)

# Add the FFmpeg function library
add_library(encoder STATIC
    src/encoder.cpp
)
# Set the target properties for encoder (where libencoder.a will be created)
set_target_properties(encoder PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/)

# Add the ImGui library
add_library(imgui STATIC
    third_party/imgui/imgui.cpp 
    third_party/imgui/imgui_draw.cpp 
    third_party/imgui/imgui_tables.cpp 
    third_party/imgui/imgui_widgets.cpp
    third_party/imgui/imgui_impl_opengl3.cpp
    third_party/imgui/imgui_impl_glfw.cpp
    third_party/imgui/imgui_demo.cpp
)
# Set the target properties for imgui (where libimgui.a will be created)
set_target_properties(imgui PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/imgui)

# Add the ImPlot library
add_library(implot STATIC 
    third_party/implot/implot.cpp
    third_party/implot/implot_items.cpp
    third_party/implot/implot_demo.cpp
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

# Add the Config library
add_library(config STATIC
    src/Config.cpp
)

# Set the target properties for config (where libconfig.a will be created)
set_target_properties(config PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/)

# Link config into any target that needs it
target_link_libraries(encoder PUBLIC config)
target_link_libraries(text PUBLIC config)

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
    pkg_check_modules(FREETYPE2 REQUIRED IMPORTED_TARGET
        freetype2
    )
    target_link_libraries(${PROJECT_NAME}
        timer
        config # don't even need to link as config already linked to encoder and text, but no harm in including again
        imgui  # ImGui static library from CMake (don't even need to link as ImPlot already linked to ImGui, but no harm in including again)
        implot # ImPlot static library from CMake
        encoder
        text
        OpenGL::GL
        glfw
        PkgConfig::FFMPEG
        PkgConfig::FREETYPE2
    )
endif(UNIX)

if(WIN32)
    target_link_libraries(${PROJECT_NAME}
        imgui  # ImGui static library from CMake (don't even need to link as ImPlot already linked to ImGui, but no harm in including again)
        implot # ImPlot static library from CMake
        encoder
        text
        timer

        #glfw  # GLFW target from CMake (alternative to libglfw3.a)
        #OpenGL::GL  # OpenGL target from CMake
        ${CMAKE_SOURCE_DIR}/lib/GLFW/libglfw3.a
        ${CMAKE_SOURCE_DIR}/lib/libopengl32.a  # OpenGL

        # FFmpeg
        ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libavformat.a
        ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libavcodec.a
        ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libavutil.a
        ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libswscale.a
        ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libswresample.a
        ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libpthread.a
        ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libm.a
        ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libws2_32.a
        ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libiconv.a
        ${CMAKE_SOURCE_DIR}/lib/ffmpeg/liblzma.a
        ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libole32.a
        ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libbcrypt.a
        ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libuuid.a
        ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libstrmiids.a

        # Freetype
        ${CMAKE_SOURCE_DIR}/lib/freetype/libfreetype.a
        ${CMAKE_SOURCE_DIR}/lib/freetype/libharfbuzz.a
        ${CMAKE_SOURCE_DIR}/lib/freetype/libgraphite2.a
        ${CMAKE_SOURCE_DIR}/lib/freetype/libpng.a
        ${CMAKE_SOURCE_DIR}/lib/freetype/libz.a
        ${CMAKE_SOURCE_DIR}/lib/freetype/libbz2.a
        ${CMAKE_SOURCE_DIR}/lib/freetype/libbrotlidec.a
        ${CMAKE_SOURCE_DIR}/lib/freetype/libbrotlienc.a
        ${CMAKE_SOURCE_DIR}/lib/freetype/libbrotlicommon.a
        ${CMAKE_SOURCE_DIR}/lib/freetype/librpcrt4.a
        ${CMAKE_SOURCE_DIR}/lib/freetype/libgdi32.a
    )
endif(WIN32)

# Set output directory for the executable
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Add a custom 'clean' target
#add_custom_target(clean-all
#    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/*
#    COMMENT "Cleaning up the build directory"
#)

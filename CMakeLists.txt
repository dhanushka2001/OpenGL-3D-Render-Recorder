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
    ${CMAKE_SOURCE_DIR}/lib/GLFW
    ${CMAKE_SOURCE_DIR}/lib/ffmpeg
    ${CMAKE_SOURCE_DIR}/lib/freetype
    ${CMAKE_SOURCE_DIR}/lib/imgui
)

# Executable target
add_executable(${PROJECT_NAME} ${SOURCES})

# Define the source and destination directories for shaders
set(SHADER_SOURCE_DIR ${CMAKE_SOURCE_DIR}/shaders)
set(SHADER_DEST_DIR   ${CMAKE_BINARY_DIR}/shaders)

# Create a custom command to copy shaders
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    	${SHADER_SOURCE_DIR} ${SHADER_DEST_DIR}
    COMMENT "Copying /shaders/ to build directory"
)

# Define the source and destination directories for textures
set(TEXTURE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/textures)
set(TEXTURE_DEST_DIR   ${CMAKE_BINARY_DIR}/textures)

# Create a custom command to copy textures
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${TEXTURE_SOURCE_DIR} ${TEXTURE_DEST_DIR}
    COMMENT "Copying /textures/ to build directory"
)

# Find all .a files in lib directory recursively
# file(GLOB_RECURSE LIB_FILES "${CMAKE_SOURCE_DIR}/lib/**/*.a")

# Link all the found libraries
target_link_libraries(${PROJECT_NAME} #${LIB_FILES}
    ${CMAKE_SOURCE_DIR}/lib/imgui/libimgui.a
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
    ${CMAKE_SOURCE_DIR}/lib/GLFW/libglfw3.a
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
    ${CMAKE_SOURCE_DIR}/lib/libopengl32.a
)
    

# Set output directory for the executable
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Add a custom 'clean' target
#add_custom_target(clean-all
#    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/*
#    COMMENT "Cleaning up the build directory"
#)

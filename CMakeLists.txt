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

# Find GLFW (make sure it's installed on your system)
#find_package(GLFW3 REQUIRED)

# Find OpenGL (on Windows, it'll look for OpenGL32.lib; on Linux, it looks for libGL)
#find_package(OpenGL REQUIRED)    

# Include headers for FFmpeg and others
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/freetype2
    ${CMAKE_SOURCE_DIR}/lib/GLFW
    ${CMAKE_SOURCE_DIR}/lib/ffmpeg
    ${CMAKE_SOURCE_DIR}/lib/freetype
    #${CMAKE_SOURCE_DIR}/lib/imgui
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

# Link all the found libraries
target_link_libraries(${PROJECT_NAME}
    imgui  # ImGui static library from CMake
    implot # ImPlot static library from CMake

    #glfw  # GLFW target from CMake (alternative to libglfw3.a)
    #OpenGL::GL  # OpenGL target from CMake

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

    ${CMAKE_SOURCE_DIR}/lib/libopengl32.a  # OpenGL
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

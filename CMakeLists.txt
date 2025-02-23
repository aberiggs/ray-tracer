# CMake version
cmake_minimum_required(VERSION 3.10)

# Project name + version
project(ray-tracer VERSION 0.0)

# C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Create compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Output directories for compiled binaries and libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include directories
include_directories(include)

# Add imgui includes
include_directories(ext/imgui ext/imgui/backends ext/imgui/misc/cpp)

# Add stb_image includes
include_directories(ext/stb)

# Add imgui sources
file(GLOB IMGUI_SOURCES
    ext/imgui/*.cpp
    ext/imgui/backends/imgui_impl_glfw.cpp
    ext/imgui/backends/imgui_impl_opengl3.cpp
    ext/imgui/misc/cpp/imgui_stdlib.cpp
)


# Build
file (GLOB_RECURSE SRC
    src/*.cpp
)
add_executable(ray_tracer ${SRC} ${IMGUI_SOURCES})
target_compile_options(ray_tracer PRIVATE -Wall -Wextra)

# Add GLFW
find_package(glfw3 3.3 REQUIRED)
target_link_libraries(ray_tracer glfw)

# Add OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(ray_tracer OpenGL::GL)

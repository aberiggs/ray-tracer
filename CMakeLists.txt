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

# Build
file (GLOB_RECURSE SRC
    src/*.cpp
)
add_executable(ray_tracer ${SRC})
target_compile_options(ray_tracer PRIVATE -Wall -Wextra)

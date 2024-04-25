cmake_minimum_required(VERSION 3.10)

project(nbody)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(nbody-mpi src/main.cpp)

target_include_directories(
    nbody-mpi PUBLIC
    "${PROJECT_SOURCE_DIR}/src"
)

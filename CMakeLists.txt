cmake_minimum_required(VERSION 3.10)

project(nbody LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# fetch latest argparse
include(FetchContent)
FetchContent_Declare(argparse
                     GIT_REPOSITORY https://github.com/p-ranav/argparse.git)
FetchContent_MakeAvailable(argparse)

find_package(UPCXX)

add_subdirectory(src)

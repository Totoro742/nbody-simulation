add_executable(nbody-mpi main.cpp)

target_include_directories(nbody-mpi PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(algorithms)
add_subdirectory(utils)

add_executable(nbody-mpi main.cpp)

target_include_directories(nbody-mpi PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(algorithms)
add_subdirectory(node)
add_subdirectory(utils)

target_link_libraries(nbody-mpi argparse)

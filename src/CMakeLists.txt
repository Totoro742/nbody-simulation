add_executable(nbody-upcxx main.cpp)

target_include_directories(nbody-upcxx PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(algorithms)
add_subdirectory(node)
add_subdirectory(utils)

target_link_libraries(nbody-upcxx argparse)
target_link_libraries(nbody-upcxx UPCXX::upcxx)

add_custom_target(clean-all COMMAND rm -rf ${CMAKE_BINARY_DIR})

target_sources(
  nbody-mpi
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/common.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/worker.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/master.cpp)

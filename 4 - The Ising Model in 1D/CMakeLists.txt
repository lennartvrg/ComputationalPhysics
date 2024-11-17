ADD_EXECUTABLE(ising_1d src/lattice_scaling_result.cpp src/main.cpp)

TARGET_COMPILE_OPTIONS(ising_1d PRIVATE -Wall -Wextra -pedantic -march=native $<$<CONFIG:Release>:-Ofast>)
TARGET_INCLUDE_DIRECTORIES(ising_1d PRIVATE includes)

TARGET_LINK_LIBRARIES(ising_1d PRIVATE common)
TARGET_LINK_LIBRARIES(ising_1d PRIVATE TBB::tbb)

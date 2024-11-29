ADD_EXECUTABLE(ising_2d_critical_slowing_down src/main.cpp)

TARGET_COMPILE_OPTIONS(ising_2d_critical_slowing_down PRIVATE -Wall -Wextra -pedantic -march=native $<$<CONFIG:Release>:-Ofast>)
TARGET_INCLUDE_DIRECTORIES(ising_2d_critical_slowing_down PRIVATE includes)

TARGET_LINK_LIBRARIES(ising_2d_critical_slowing_down PRIVATE common)
TARGET_LINK_LIBRARIES(ising_2d_critical_slowing_down PRIVATE TBB::tbb)

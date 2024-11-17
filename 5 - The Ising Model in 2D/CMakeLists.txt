ADD_EXECUTABLE(ising_2d src/main.cpp)

TARGET_COMPILE_OPTIONS(ising_2d PRIVATE -Wall -Wextra -pedantic -march=native $<$<CONFIG:Release>:-Ofast>)
TARGET_INCLUDE_DIRECTORIES(ising_2d PRIVATE includes)

TARGET_LINK_LIBRARIES(ising_2d PRIVATE common)
TARGET_LINK_LIBRARIES(ising_2d PRIVATE TBB::tbb)

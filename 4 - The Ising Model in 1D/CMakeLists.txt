CMAKE_MINIMUM_REQUIRED(VERSION 3.25)
PROJECT(main LANGUAGES CXX)

SET(CMAKE_CXX_STANDARD 23)
FIND_PACKAGE(TBB REQUIRED)

INCLUDE_DIRECTORIES(includes)
ADD_EXECUTABLE(main src/lattice.cpp src/lattice_1d.cpp src/lattice_measurement.cpp src/utils.cpp src/main.cpp)

TARGET_LINK_LIBRARIES(main PRIVATE TBB::tbb)
TARGET_COMPILE_OPTIONS(main PRIVATE -Wall -Wextra -pedantic $<$<CONFIG:Release>:-Ofast>)

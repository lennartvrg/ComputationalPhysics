CMAKE_MINIMUM_REQUIRED(VERSION 3.25)
PROJECT(main LANGUAGES CXX)

SET(CMAKE_CXX_STANDARD 26)
FIND_PACKAGE(TBB REQUIRED)

INCLUDE_DIRECTORIES(../Libraries/includes)
ADD_EXECUTABLE(main ../Libraries/src/histogram.cxx src/main.cpp)

TARGET_LINK_LIBRARIES(main PRIVATE TBB::tbb)
TARGET_COMPILE_OPTIONS(main PRIVATE -Wall -Wextra -pedantic $<$<CONFIG:Release>:-Ofast>)

CMAKE_MINIMUM_REQUIRED(VERSION 3.25)

IF (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(HOMEBREW_PREFIX "/usr/local" CACHE PATH "Path to Homebrew installation")
    SET(CMAKE_C_COMPILER "${HOMEBREW_PREFIX}/bin/gcc-14")
    SET(CMAKE_CXX_COMPILER "${HOMEBREW_PREFIX}/bin/g++-14")
ENDIF()

PROJECT(main LANGUAGES CXX)

SET(CMAKE_CXX_STANDARD 23)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

INCLUDE_DIRECTORIES(includes)
ADD_EXECUTABLE(main src/lattice.cpp src/lattice_1d.cpp src/lattice_result.cpp src/metropolis_result.cpp src/utils.cpp src/main.cpp
        includes/utils.h)
TARGET_COMPILE_OPTIONS(main PRIVATE -Wall -Wextra -pedantic -march=native $<$<CONFIG:Release>:-Ofast>)

find_package(TBB REQUIRED)
target_link_libraries(main PRIVATE TBB::tbb)

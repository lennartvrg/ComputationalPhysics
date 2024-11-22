ADD_LIBRARY(common src/histogram.cpp src/lattice.cpp src/lattice_1d.cpp src/lattice_2d.cpp src/metropolis_result.cpp src/utils.cpp
        includes/lattice_2d.h
        includes/lattice_observable.h)

TARGET_INCLUDE_DIRECTORIES(common PUBLIC includes)
TARGET_COMPILE_OPTIONS(common PRIVATE -Wall -Wextra -pedantic -march=native $<$<CONFIG:Release>:-Ofast>)

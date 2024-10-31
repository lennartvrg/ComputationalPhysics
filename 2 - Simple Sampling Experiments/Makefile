CXXFLAGS = -Ofast -std=c++23 -Wall -Wextra -pedantic

ifeq ($(OS),Windows_NT)
    CXXFLAGS += -ltbb
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        CXXFLAGS += -ltbb
    endif
    ifeq ($(UNAME_S),Darwin)
        CXXFLAGS += -fexperimental-library
    endif
endif


all: main

clean:
	rm -f main

main: src/main.cpp
	c++ $(CXXFLAGS) -o main src/main.cpp

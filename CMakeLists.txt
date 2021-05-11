cmake_minimum_required(VERSION 3.13)
project(SequencePairs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER clang++-12)
add_definitions(-Wall -Wextra -pedantic -Werror)

add_executable(SequencePairs
    src/main.cpp
    src/Permutation.cpp src/Permutation.h
    src/Digraph.cpp src/Digraph.h
    src/Instance.cpp src/Instance.h
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

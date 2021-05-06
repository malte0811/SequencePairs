cmake_minimum_required(VERSION 3.13)
project(SequencePairs)

set(CMAKE_CXX_STANDARD 17)

add_executable(SequencePairs
    src/main.cpp
    src/Permutation.cpp src/Permutation.h
    src/Digraph.cpp src/Digraph.h
    src/Instance.cpp src/Instance.h
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

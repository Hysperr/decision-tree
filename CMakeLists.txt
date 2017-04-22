cmake_minimum_required(VERSION 3.7)
project(decision_tree)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        main.cpp
        Node.cpp
        Node.h
        README.md
        tree.xml)

add_executable(decision_tree ${SOURCE_FILES})

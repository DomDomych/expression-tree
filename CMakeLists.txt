cmake_minimum_required(VERSION 3.10)

project(expression_tree)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(expression_tree
    example/main.cpp
    tree/tree.cpp
)

target_include_directories(expression_tree PRIVATE
    tree
    map
)
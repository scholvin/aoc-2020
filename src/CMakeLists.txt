cmake_minimum_required(VERSION 3.10)

project(aoc-2020)

set(BOOST_ROOT /usr/include/boost169)
find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})

add_compile_options(-std=c++2a -ggdb -Wall)  # -Og makes it faster, but hard to debug

add_executable(aoc-2020 main.cpp runner.cpp week1.cpp week2.cpp)

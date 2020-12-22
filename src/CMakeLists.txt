cmake_minimum_required(VERSION 3.10)

project(aoc-2020)

set(BOOST_ROOT /usr/include/boost169)
find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})

# TODO: include creation of the bison files from the data files via src/make-bison.awk, for example:
# awk -f src/make-bison.awk < data/day19a.dat > src/day19a.y
find_package(BISON)
BISON_TARGET(Day19a day19a.y ${CMAKE_CURRENT_BINARY_DIR}/day19a.cpp COMPILE_FLAGS "--name-prefix=day19a -Wno-conflicts-sr -Wno-conflicts-rr")
BISON_TARGET(Day19b day19b.y ${CMAKE_CURRENT_BINARY_DIR}/day19b.cpp COMPILE_FLAGS "--name-prefix=day19b -Wno-conflicts-sr -Wno-conflicts-rr")

add_compile_options(-std=c++2a -ggdb -Wall)  # -Og makes it faster, but hard to debug

add_executable(aoc-2020 main.cpp runner.cpp week1.cpp week2.cpp week3.cpp week4.cpp ${BISON_Day19a_OUTPUTS} ${BISON_Day19b_OUTPUTS})

# AOC 2020

This is my code for the [Advent of Code 2020 challenge](https://adventofcode.com/2020).

I'm not really making any effort for this code to be efficient. Just trying to get the answers right.

I'm testing this with gcc 8.3.1 and cmake 3.11.4. Starting with Day 7, I added boost 1.69.
* It'll probably work with a lot of other compilers. 
* I set the experimental C++20 flag, but it's unlikely I'll use any of those features.

## To build:
```
mkdir build
cd build
cmake ../src
make
```

This generates the `aoc-2020` executable in the build directory. Run it from there, as it will look to `../data` for input files.
Command line help is available from the executable.


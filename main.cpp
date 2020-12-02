#include <iostream>
#include <string>

#include "basic.h"

int main(int argc, char** argv)
{
    if (argc == 1)
    {
        std::cerr << "usage: aoc-2020 id [id id id...]" << std::endl;
        return 1;
    }
    for (int i = 1; i < argc; ++i)
    {
        const std::string id(argv[i]);
        basic::result result = basic::run(id);
        if (result.first)
            std::cout << id << " answer: " << result.second << std::endl;
        else
            std::cout << id << " no function found" << std::endl;
    }
    return 0;
}

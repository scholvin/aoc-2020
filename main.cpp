#include <iostream>

#include "basic.h"

void runner(const std::string& id)
{
    if (id == "1a")
        std::cout << day01a() << std::endl;
    else if (id == "1b")
        std::cout << day01b() << std::endl;
    else
        std::cerr << "unknown id " << id << std::endl;
}

int main(int argc, char** argv)
{
    if (argc == 1)
    {
        std::cerr << "usage: aoc-2020 id [id id id...]" << std::endl;
        return 1;
    }
    for (int i = 1; i < argc; ++i)
    {
        runner(argv[i]);
    }
    return 0;
}

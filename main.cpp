#include <iostream>
#include <string>
#include <map>

#include "basic.h"

typedef int (*SimpleFunc)(void);
typedef std::map<std::string, SimpleFunc> SimpleMap;

const SimpleMap simple = {
    { "1a", day01a },
    { "1b", day01b }
};

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
        auto sit = simple.find(id);
        if (sit != simple.end())
        {
            int answer = (sit->second)();
            std::cout << id << " answer: " << answer << std::endl;
        }
        else
        {
            std::cout << id << " no function found" << std::endl;
        }
    }
    return 0;
}

#include "basic.h"

#include <iostream>
#include <regex>
#include <algorithm>

// would prefer to have these added automatically somehow
const std::map<std::string, std::function<long(void)>> basic::method_map =
{
    { "1a", &basic::day01a },
    { "1b", &basic::day01b },
    { "2a", &basic::day02a },
    { "2b", &basic::day02b },
    { "3a", std::bind(&basic::day03a, 3, 1) },
    { "3b", &basic::day03b }
};

void basic::run(const std::string& id)
{
    auto it = method_map.find(id);
    if (it != method_map.end())
    {
        long answer = (it->second)();
        std::cout << id << " answer: " << answer << std::endl;
    }
    else
    {
        std::cout << "id " << id << " is not recognized" << std::endl;
    }
}

void basic::run_all()
{
    for (auto it: method_map)
    {
        long answer = (it.second)();
        std::cout << it.first << " answer: " << answer << std::endl;
    }
}

std::string basic::get_keys()
{
    std::string ret = "";
    for (auto it: method_map)
    {
        ret += it.first + " ";
    }
    return ret;
}

const int day01target = 2020;

long basic::day01a()
{
    // return the product of the two entries that sum to 2020. O(N^2), don't care.
    for (auto i = day01input.begin(); i != day01input.end(); i++)
    {
        for (auto j = i + 1; j != day01input.end(); j++)
        {
            if (*i + *j == day01target)
            {
                //std::cout << *i << " " << *j << std::endl;
                return *i * *j;
            }
        }
    }

    // shouldn't happen
    return -1;
}

long basic::day01b()
{
    // return the product of the three entries that sum to 2020. O(N^3), don't care.
    for (auto i = day01input.begin(); i != day01input.end(); i++)
    {
        for (auto j = i + 1; j != day01input.end(); j++)
        {
            for (auto k = j + 1; k != day01input.end(); k++)
            {
               if (*i + *j + *k == day01target)
               {
                    //std::cout << *i << " " << *j << " " << *k << std::endl;
                    return *i * *j * *k;
               }
            }
        }
    }

    // shouldn't happen
    return -1;
}      

long basic::day02worker(day02func func)
{
    long valid = 0;
    std::regex r("([0-9]+)-([0-9]+) ([a-z]): ([a-z]+)");
    for (auto s: day02input)
    {
        std::smatch m;
        std::regex_search(s, m, r);
        int lo = std::stoi(m.str(1));
        int hi = std::stoi(m.str(2));
        char ch = m.str(3)[0];
        const std::string& pw = m.str(4);

        if (func(lo, hi, ch, pw))
            valid++;
    }
    return valid; 
}

long basic::day02a()
{
    return day02worker([](int l, int h, char ch, const std::string& pw) -> bool { int num = std::count(pw.begin(), pw.end(), ch); return (num >= l && num <= h); });
}

long basic::day02b()
{
    return day02worker([](int l, int h, char ch, const std::string& pw) -> bool { return ((pw[l-1] == ch && pw[h-1] != ch) || (pw[l-1] != ch && pw[h-1] == ch)); });
}

long basic::day03a(int right, int down)
{
    int x = 0, y = 0;
    long trees = 0;
    while (y < day03input.size())
    {
        // x/y in reverse order but correct
        if (day03input[y][x] == '#')
            trees++;
        y += down;
        x = (x + right) % day03input[0].size();
    }
    return trees;
}

long basic::day03b()
{
    const std::vector<std::vector<int>> forests = { 
        { 1, 1 },
        { 3, 1 },
        { 5, 1 },
        { 7, 1 },
        { 1, 2 }
    };
    long product = 1;
    for (auto f: forests)
        product *= day03a(f[0], f[1]);
    return product;
}

// moved the data out to data.cpp to give myself a break on compile times
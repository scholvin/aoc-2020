#include <iostream>
#include <string>

#include "basic.h"

int main(int argc, char** argv)
{
    using namespace std;
    if (argc == 1)
    {
        cerr << "usage: aoc-2020 id [id id id...]" << endl
             << "       where 'id' is something like 3a for the day 3a challenge" << endl;
        return 1;
    }
    for (int i = 1; i < argc; ++i)
    {
        const string id(argv[i]);
        basic::result result = basic::run(id);
        if (result.first)
            cout << id << " answer: " << result.second << endl;
        else
            cout << id << " no challenge found" << endl;
    }
    return 0;
}

#include <iostream>
#include "runner.h"
#include "week1.h"
#include "week2.h"

int main(int argc, char** argv)
{
    using namespace std;
    runner run;
    run.add(week1::method_map);
    run.add(week2::method_map);

    if (argc == 1)
    {
        cerr << "usage: aoc-2020 id [id id id...]" << endl
             << "       where 'id' is one of the following:" << endl
             << "           " << run.get_keys() << endl
             << "   or: aoc-2020 all" << endl;
        return 1;
    }
    if (std::string(argv[1]) == "all")
    {
        run.run_all();
    }
    else
    {
        for (int i = 1; i < argc; ++i)
        {
            run.run(argv[i]);
        }
    }
    return 0;
}


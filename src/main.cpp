#include <iostream>
#include "basic.h"
//#include "week2.h"

int main(int argc, char** argv)
{
    using namespace std;
    if (argc == 1)
    {
        cerr << "usage: aoc-2020 id [id id id...]" << endl
             << "       where 'id' is one of the following:" << endl
             << "           " << basic::get_keys() << endl
  //           << "           " << week2::get_key() << endl
             << "   or: aoc-2020 all" << endl;
        return 1;
    }
    if (std::string(argv[1]) == "all")
    {
        basic::run_all();
        //week2::run_all();
    }
    else
    {
        for (int i = 1; i < argc; ++i)
        {
            basic::run(argv[i]);
          //  week2::run(argv[i]);
        }
    }
    return 0;
}


#include "week3.h"

#include <fstream>
#include <iostream>
#include <unordered_map>

namespace week3 
{
    const std::vector<long> DAY15_DATA = { 9, 19, 1, 6, 0, 5, 4 };

    long day15(size_t turns)
    {
        std::unordered_map<long, size_t> spoken;
        size_t turn;
        for (turn = 1; turn < DAY15_DATA.size(); turn++)
            spoken[DAY15_DATA[turn-1]] = turn;
        turn++;

        long last = DAY15_DATA.back();
        while (turn <= turns)
        {
            // if this was the first time it was spoken, or the last of the starting numbers, speak 0
            // if not, speak the age since most recent spoken
            // note: first implementation, I had this way harder than it needed to be
            auto it = spoken.find(last);
            if (it == spoken.end())
            {
                // first time
                spoken[last] = turn - 1;
                last = 0;
            }
            else
            {
                // seen before
                long diff = turn - 1 - it->second;
                it->second = turn - 1;
                last = diff;
            }
            turn++;
        }
        return last;
    }
}

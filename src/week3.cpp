#include "week3.h"

#include <fstream>
#include <iostream>

namespace week3 
{
	const std::vector<long> DAY15_DATA = { 9, 19, 1, 6, 0, 5, 4 };

    long day15a()
    {
    	std::vector<long> spoken = DAY15_DATA;

    	for (size_t turn = DAY15_DATA.size() + 1; turn <= 2020; turn++)
    	{
    		long last = spoken.back();
    		// if this was the first time it was spoken, speak 0
    		// if not, speak the age since most recent spoken
    		bool found = false;
    		int idx = spoken.size() - 2;
    		while (idx >= 0)
    		{
    			if (spoken[idx] == last)
    			{
    				found = true;
    				break;
    			}
    			idx--;
    		}
    		if (found)
    		{
    			// speak age, which is turn - idx
    			spoken.push_back(turn - idx - 2);
    		}
    		else
    		{
    			// speak 0
    			spoken.push_back(0);
    		}
    	}
        return spoken.back();
    }

    long day15b()
    {
        return -1;
    }
}

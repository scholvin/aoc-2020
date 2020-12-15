#include "week3.h"

#include <fstream>
#include <iostream>
#include <unordered_map>

namespace week3 
{
	const std::vector<long> DAY15_DATA = { 9, 19, 1, 6, 0, 5, 4 };
	//const std::vector<long> DAY15_DATA = { 0, 3, 6 };
	//const std::vector<long> DAY15_DATA = { 3, 1, 2 };

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
    		//std::cout << turn << " " << spoken.back() << std::endl;
    	}
        return spoken.back();
    }

    long day15b()
    {
    	// 30000000
    	std::unordered_map<long, std::vector<size_t>> spoken;
    	size_t turn;
    	for (turn = 0; turn < DAY15_DATA.size(); turn++)
    		spoken[DAY15_DATA[turn]].push_back(turn+1);

    	long last = DAY15_DATA.back();
    	for (turn = DAY15_DATA.size() + 1; turn <= 30000000; turn++)
    	{
    		// if this was the first time it was spoken, or the last of the starting numbers, speak 0
    		// if not, speak the age since most recent spoken
    		auto it = spoken.find(last);
    		if (turn == DAY15_DATA.size() + 1 || it == spoken.end() || it->second.size() < 2)
    		{
    			// speak 0
    			last = 0;
    			spoken[0].push_back(turn);
    		}
    		else
    		{
    			// speak age
    			auto tail = it->second.rbegin();
    			last = *tail - *(tail+1);
    			spoken[last].push_back(turn);
    		}
    		if (turn % 1000000 == 0)
    			std::cout << turn << " " << last << std::endl;
    	}
    	return last;
    }
}

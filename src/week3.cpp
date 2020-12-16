#include "week3.h"

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <regex>
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>

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

    struct range
    {
        std::string tag;
        long lo1, hi1, lo2, hi2;

        // assumes range 1 is strictly less than range 2 (which appears to be the case in the data if not the spec)
        bool is_valid(long f) const { return (f >= lo1 && f <= hi1) || (f >= lo2 && f <= hi2); }
    };

    typedef std::vector<long> ticket_t;

    const std::regex RANGE_RE("([a-z ]+): ([0-9]+)-([0-9]+) or ([0-9]+)-([0-9]+)");

    long day16a()
    {
        // reading this file is a pain in the arse
        std::ifstream infile("../data/day16.dat");
        std::string line;

        std::vector<range> ranges;
        while (true)
        {
            std::getline(infile, line);
            std::smatch sm;
            if (!std::regex_search(line, sm, RANGE_RE))
                break;
            ranges.push_back(range { sm.str(1), std::stol(sm.str(2)), std::stol(sm.str(3)), std::stol(sm.str(4)), std::stol(sm.str(5)) } );
        }

        std::getline(infile, line); // throw away "your ticket:"
        std::getline(infile, line);
        ticket_t my_ticket;
        boost::tokenizer<> tok(line);
        std::transform(tok.begin(), tok.end(), std::back_inserter(my_ticket), &boost::lexical_cast<long,std::string>);

        std::getline(infile, line); // throw away blank line
        std::getline(infile, line); // throw away "nearby tickets:"
        std::vector<ticket_t> nearby_tickets;
        while (std::getline(infile, line))
        {
            ticket_t next;
            boost::tokenizer<> tok(line);
            std::transform(tok.begin(), tok.end(), std::back_inserter(next), &boost::lexical_cast<long,std::string>);
            nearby_tickets.push_back(next);
        }

        long sum = 0;
        for (auto t: nearby_tickets)
        {
            for (auto v: t)
            {
                bool valid = false;
                // the ticket values thare not valid in any range
                for (auto r: ranges)
                {
                    if (r.is_valid(v))
                    {
                        valid = true;
                        break;
                    }
                }
                if (!valid)
                {
                    sum += v;
                    break;
                }

            }
        }

        return sum;
    }
}

#include "week3.h"

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <regex>
#include <algorithm>
#include <set>
#include <unordered_set>

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/container_hash/hash.hpp>

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

        // for debugging
        std::string str() const
        {
            return tag + ": " + std::to_string(lo1) + "/" + std::to_string(hi1) + " "
                       + std::to_string(lo2) + "/" + std::to_string(hi2);
        }
    };

    typedef std::vector<long> ticket_t;
    typedef std::vector<ticket_t> tickets_t;
    typedef std::vector<range> ranges_t;

    // if any field is invalid, the ticket is invalid
    // if every field is valid, the ticket is valid
    bool is_ticket_valid(const ranges_t& ranges, const ticket_t ticket)
    {
        for (auto value: ticket)
        {
            bool valid = false;
            for (auto r: ranges)
            {
                if (r.is_valid(value))
                {
                    valid = true;
                    break;
                }
            }
            if (!valid)
                return false;
        }
        return true;
    }

    const std::regex RANGE_RE("([a-z ]+): ([0-9]+)-([0-9]+) or ([0-9]+)-([0-9]+)");

    void day16read(ranges_t& ranges, ticket_t& my_ticket, tickets_t& nearby_tickets)
    {
        // reading this file is a pain in the arse
        std::ifstream infile("../data/day16.dat");
        std::string line;

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
        boost::tokenizer<> tok(line);
        std::transform(tok.begin(), tok.end(), std::back_inserter(my_ticket), &boost::lexical_cast<long,std::string>);

        std::getline(infile, line); // throw away blank line
        std::getline(infile, line); // throw away "nearby tickets:"
        while (std::getline(infile, line))
        {
            ticket_t next;
            boost::tokenizer<> tok(line);
            std::transform(tok.begin(), tok.end(), std::back_inserter(next), &boost::lexical_cast<long,std::string>);
            nearby_tickets.push_back(next);
        }
    }

    long day16a()
    {
        std::vector<range> ranges;
        ticket_t my_ticket;
        tickets_t nearby_tickets;
        day16read(ranges, my_ticket, nearby_tickets);

        long sum = 0;
        for (auto t: nearby_tickets)
        {
            for (auto v: t)
            {
                bool valid = false;
                // find the ticket values that are not valid in any range
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

    long day16b()
    {
        std::vector<range> ranges;
        ticket_t my_ticket;
        tickets_t nearby_tickets;
        day16read(ranges, my_ticket, nearby_tickets);

        // discard invalid tickets
        tickets_t valid_tickets;
        for (auto t: nearby_tickets)
            if (is_ticket_valid(ranges, t))
                valid_tickets.push_back(t);

        const size_t POSITIONS = ranges.size(); // for clarity

        // this vector maps position to a set of possible solutions
        std::vector<std::set<size_t>> solutions(POSITIONS);
        // initialize each position to all possible solutions
        for (size_t s = 0; s < POSITIONS; s++)
            for (size_t r = 0; r < POSITIONS; r++)
                solutions[s].insert(r);

        // eliminate possible solutions where a position matches no rules
        // this is something like O(positions * tickets * rules * log(positions))
        for (size_t pos = 0; pos < POSITIONS; pos++)
            for (auto t: valid_tickets)
                for (size_t r = 0; r < ranges.size(); r++)
                    if (!ranges[r].is_valid(t[pos]))
                        solutions[pos].erase(r);

        // now we do process of elimination - find solutions with only 1 range, and then remove that range from everyone else
        // note, if that's not how this is set up, this thing may never terminate
        // this is something like O(positions * positions * log(positions))
        std::vector<size_t> final(POSITIONS, std::numeric_limits<size_t>::max());
        while (true)
        {
            size_t s = 0;
            while (solutions[s].size() != 1 && s < POSITIONS)
                s++;
            if (s == POSITIONS)
                break;
            final[s] = *(solutions[s].begin());
            for (size_t x = 0; x < POSITIONS; x++)
                solutions[x].erase(final[s]);
        }

        // finally, dereference my_ticket values as the question asks
        long product = 1;
        for (size_t t = 0; t < POSITIONS; t++)
            if (ranges[final[t]].tag.substr(0, 9) == "departure")
                product *= my_ticket[t];

        return product;
    }

    long day17a()
    {
        /* 
            cheat alert!

            Since multi-dimensional C-style arrays are an absolute nightmare, I am using std::array.
            But that requires knowing the array size at compile time. So I totally, 100% cheated here,
            and looked at the data file to set GRID below.

            This approach also makes it nearly impossible to generalize for the second part.

            Another option would be to just track some data structure of active points, maybe a hash. 
            I will try that in the alt implementation. Betting it won't be as fast as this, but it will
            use less memory, and be more in the spirit of the challenge, maybe.
        */

        const size_t GRID = 8;
        const size_t GENERATIONS = 6;
        const size_t N = GRID + GENERATIONS * 2;
        typedef std::array<std::array<std::array<bool, N>, N>, N> universe_t;

        std::ifstream infile("../data/day17.dat");
        std::string line;

        universe_t prev = {};
        size_t y_p = GENERATIONS, z_p = N / 2;
        while (std::getline(infile, line))
        {
            for (size_t x_p = 0; x_p < line.size(); x_p++)
            {
                if (line[x_p] == '#')
                    prev[x_p + GENERATIONS][y_p][z_p] = true;
            }
            y_p++;
        }

        long active;
        for (size_t g = 1; g <= GENERATIONS; g++)
        {
            active = 0;
            universe_t curr{};
            // for each cell (x0, y0, z0) in the previous generation:
            for (size_t x0 = 0; x0 < N; x0++)
            {
                for (size_t y0 = 0; y0 < N; y0++)
                {
                    for (size_t z0 = 0; z0 < N; z0++)
                    {
                        // find all the active neighbors of (x0, y0, z0)
                        long neighbors = 0;
                        for (size_t x = x0 - 1; x <= x0 + 1; x++)
                        {
                            for (size_t y = y0 - 1; y <= y0 + 1; y++)
                            {
                                for (size_t z = z0 - 1; z <= z0 + 1; z++)
                                {
                                    // since these are size_t's, they'll never be < 0, and >= N is out of range, too
                                    // also, don't eval the cell in the middle, that's the starting point
                                    if (x >= N || y >= N || z >= N || (x == x0 && y == y0 && z == z0))
                                        continue;
                                    if (prev[x][y][z])
                                        neighbors++;
                                    if (neighbors == 4)
                                        goto toomany; // we can stop evaluating this cell
                                }
                            }
                        }
toomany:
                        // now we know # of active neighbors
                        if (prev[x0][y0][z0] && (neighbors == 2 || neighbors == 3))
                        {
                            curr[x0][y0][z0] = true;
                            active++;
                        }
                        else if (!prev[x0][y0][z0] && neighbors == 3)
                        {
                            curr[x0][y0][z0] = true;
                            active++;
                        }
                        else
                            curr[x0][y0][z0] = false; // this may be wrong, or unnecessary
                    }
                }
            }
            prev = curr;
        }

        return active;
    }

    long day17b()
    {
        const size_t GRID = 8;
        const size_t GENERATIONS = 6;
        const size_t N = GRID + GENERATIONS * 2;
        // see note in 17a about how this is basically not generalizable without insane metaprogramming
        typedef std::array<std::array<std::array<std::array<bool, N>, N>, N>, N> universe_t;

        std::ifstream infile("../data/day17.dat");
        std::string line;

        universe_t prev = {};
        size_t y_p = GENERATIONS, z_p = N / 2, w_p = N / 2;
        while (std::getline(infile, line))
        {
            for (size_t x_p = 0; x_p < line.size(); x_p++)
            {
                if (line[x_p] == '#')
                    prev[x_p + GENERATIONS][y_p][z_p][w_p] = true;
            }
            y_p++;
        }

        long active;
        for (size_t g = 1; g <= GENERATIONS; g++)
        {
            active = 0;
            universe_t curr{};
            // for each cell (x0, y0, z0, w0) in the previous generation:
            for (size_t x0 = 0; x0 < N; x0++)
            {
                for (size_t y0 = 0; y0 < N; y0++)
                {
                    for (size_t z0 = 0; z0 < N; z0++)
                    {
                        for (size_t w0 = 0; w0 < N; w0++)
                        {
                            // find all the active neighbors of (x0, y0, z0, w0)
                            long neighbors = 0;
                            for (size_t x = x0 - 1; x <= x0 + 1; x++)
                            {
                                for (size_t y = y0 - 1; y <= y0 + 1; y++)
                                {
                                    for (size_t z = z0 - 1; z <= z0 + 1; z++)
                                    {
                                        for (size_t w = w0 - 1; w <= w0 + 1; w++)
                                        {
                                            // since these are size_t's, they'll never be < 0, and >= N is out of range, too
                                            // also, don't eval the cell in the middle, that's the starting point
                                            if (x >= N || y >= N || z >= N || w >= N || (x == x0 && y == y0 && z == z0 && w == w0))
                                                continue;
                                            if (prev[x][y][z][w])
                                                neighbors++;
                                            if (neighbors == 4)
                                                goto toomany; // we can stop evaluating this cell
                                        }
                                    }
                                }
                            }
toomany:
                            // now we know # of active neighbors
                            if (prev[x0][y0][z0][w0] && (neighbors == 2 || neighbors == 3))
                            {
                                curr[x0][y0][z0][w0] = true;
                                active++;
                            }
                            else if (!prev[x0][y0][z0][w0] && neighbors == 3)
                            {
                                curr[x0][y0][z0][w0] = true;
                                active++;
                            }
                            else
                                curr[x0][y0][z0][w0] = false; // this may be wrong, or unnecessary
                        }
                    }
                }
            }
            prev = curr;
        }

        return active;
    }
}

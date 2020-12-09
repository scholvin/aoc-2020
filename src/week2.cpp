#include "week2.h"

#include <regex>
#include <set>
#include <fstream>
#include <iostream>

namespace week2 
{
    enum machine_code {
        nop,
        acc,
        jmp
    };

    typedef std::vector<std::pair<machine_code, long>> program_t;

    void read_program(program_t& program, const std::string& filename)
    {
        const std::map<std::string, machine_code> translator =
        {
            { "nop", nop },
            { "acc", acc },
            { "jmp", jmp }
        };

        std::ifstream infile(filename);
        std::string line;

        const std::regex line_re("([a-z]+) ([-+][0-9]+)");
        while (std::getline(infile, line))
        {
            std::smatch sm;
            std::regex_search(line, sm, line_re);
            auto it = translator.find(sm.str(1));
            if (it != translator.end())
            {
                program.push_back(std::make_pair(it->second, std::stol(sm.str(2))));
            }
            else
                throw std::runtime_error("bad code: ");
        }
    }

    void run_program(const program_t& program, long& accumulator, bool& success)
    {
        std::set<size_t> seen;

        // run the program
        success = false;
        accumulator = 0;
        size_t program_counter = 0;
        while (program_counter < program.size())
        {
            //std::cout << "pc=" << program_counter << " ac=" << accumulator << " op=" << program[program_counter].first << " arg=" << program[program_counter].second << std::endl;
            if (seen.find(program_counter) != seen.end())
            {
                return;
            }
            seen.insert(program_counter);

            switch (program[program_counter].first)
            {
                case nop: program_counter++; break;
                case acc: accumulator += program[program_counter++].second; break;
                case jmp: program_counter += program[program_counter].second; break;
            }
        }

        success = true;
    }

    long day08a()
    {
        program_t program;
        read_program(program, "../data/day08.dat");

        long accumulator;
        bool success;
        run_program(program, accumulator, success);
        return accumulator;
    }

    long day08b()
    {
        program_t program;
        read_program(program, "../data/day08.dat");

        long accumulator;
        bool success;

        for (size_t i = 0; i < program.size(); i++)
        {
            if (program[i].first == nop || program[i].first == jmp)
            {
                program_t copy = program;
                if (program[i].first == nop)
                    copy[i].first = jmp;
                else
                    copy[i].first = nop;
                run_program(copy, accumulator, success);
                if (success)
                    return accumulator;
            }
        }
        // shouldn't happen
        return -1;
    }

    const size_t PREAMBLE = 25;

    long day09a()
    {
        std::ifstream infile("../data/day09.dat");
        std::string line;
        std::deque<long> queue;

        // seed
        for (size_t i = 0; i < PREAMBLE; ++i)
        {
            std::getline(infile, line);
            queue.push_back(std::stol(line));
        }

        while (std::getline(infile, line))
        {
            long next = std::stol(line);
            bool found = false;
            // O(N^2), don't care
            for (size_t i = 0; i < PREAMBLE; ++i)
                for (size_t j = 1; j < PREAMBLE; ++j)
                    if (queue[i] + queue[j] == next)
                    {
                        found = true;
                        break;
                    }
            if (!found)
                return next;
            queue.pop_front();
            queue.push_back(next);
        }

        // shouldn't happen
        return -1;
    }

    long day09b()
    {
        std::ifstream infile("../data/day09.dat");
        std::string line;
        std::vector<long> queue;

        while (std::getline(infile, line))
        {
            long next = std::stol(line);
            queue.push_back(next);
        }

        const long invalid_sum = day09a();  // ~25ms

        // brute force, another O(N^2), surely there's an O(N) way to do this by wiggling the ends of a range
        for (size_t i = 0; i < queue.size(); ++i)
        {
            long working_sum = queue[i];
            for (size_t j = i+1; j < queue.size(); ++j)
            {
                working_sum += queue[j];
                if (working_sum > invalid_sum)
                {
                    break;
                }
                if (working_sum == invalid_sum)
                {
                    // done, now find smallest and largest, which are NOT first and last
                    long min = std::numeric_limits<long>::max();
                    long max = 0;
                    for (size_t k = i; k <= j; k++)
                    {
                        if (queue[k] > max) max = queue[k];
                        if (queue[k] < min) min = queue[k];
                    }
                    return min + max;
                }
            }
        }

        // shouldn't happen
        return -1;
    }
};
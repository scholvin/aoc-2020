#include "week2.h"

#include <regex>
#include <set>
#include <fstream>

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
};
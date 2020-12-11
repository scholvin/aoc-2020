#include "week2.h"

#include <regex>
#include <set>
#include <fstream>
#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/topological_sort.hpp>

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

        const long target = day09a();  // ~25ms

        size_t i = 0, j = 1;
        long working_sum = queue[i] + queue[j];

        while ((working_sum != target) && j < queue.size() && i < queue.size())
        {
            if (working_sum < target)
                working_sum += queue[++j];
            else if (working_sum > target)
                working_sum -= queue[i++];
        }

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

    long day10a()
    {
        std::ifstream infile("../data/day10.dat");
        std::string line;
        std::set<long> adapters;
        while (std::getline(infile, line))
        {
            adapters.insert(std::stol(line));
        }

        std::vector<long> gaps(3, 0);

        long jolt = 0;
        for (auto a: adapters)
        {
            long diff = a - jolt;
            gaps[diff-1]++;
            jolt = a;
        }
        gaps[2]++; // final adadpter is device
        return gaps[0] * gaps[2];
    }


    long day10b()
    {
        std::ifstream infile("../data/day10.dat");
        std::string line;

        struct vertex_prop_t { long jolt; boost::default_color_type color; };
        using graph_t = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, vertex_prop_t>;
        using vertex_t = boost::graph_traits<graph_t>::vertex_descriptor;

        /*
            This was a journey. There is surely a better way, and a better data structure, though this
            was pretty sparky, ultimately running in < 1ms. I'm putting this comment here for posterity, 
            though it will surely be longer than the code.

            STRATEGY:

            1. load the data into a directed, acyclic graph such that:
                a. each vertex represents an adapter, plus one for the base 0, and one more for the device
                b. vertex property is the actual jolt value, though that's not necessary after the graph is built
                c. each edge represents a valid transition with the 3 jolt restriction, i.e., if the vertices
                   are 0, 1, 2, and 5, the edges will be (0,1) (0,2) and (2,5). No edge from (1,5) because it's more
                   than 3 jolts.
                d. set u = start vertex, v = finish vertex
            2. do a topological sort on the graph into a vector (call it "sorted")
                a. FUN: I originally used a deque, because it has a front_inserter and a [] operator, but I was
                   having grave difficulties with the operator; that is, it appeared not to fucking work. So
                   I went back to a vector which only has back_inserter, the result being that the resultant
                   topo sort will be in reverse order. This iwas fine, because:
            3. now do some dynamic programming, ish, based on the article here: 
                https://www.geeksforgeeks.org/number-of-paths-from-source-to-destination-in-a-directed-acyclic-graph/
                Paraphrasing:
                    let f(u) be the number of ways you can get from node u to the end
                    for the end node, f(u) = 1
                    working back toward the front of the graph, at any node u, f(u) = the sum of f(d) for all nodes d
                    immedately downstream from u

                the algorithm is basically:

                create a map (call it dp) from vertex -> possible downstream paths
                initialize dp[s] = 0 for all vertices s in the graph
                set dp[v] = 1
                for each vertex t in the topo sort
                    for each vertex o reachable from t
                        dp[t] += dp[o]
                return dp[u]

            This code mostly sucks because of the boost interface, I think. The idea is pretty simple. But it's late
            and I'm leaving it.
        */

        // ugh - usual boost problem of tracking your own vertices while building
        std::map<long, vertex_t> vmap;
        graph_t graph;

        // initial node
        auto u = boost::add_vertex(vertex_prop_t { 0 }, graph);
        vmap[0] = u;

        // read the rest of the nodes
        while (std::getline(infile, line))
        {
            long jolt = std::stol(line);
            auto n = boost::add_vertex(vertex_prop_t{ jolt }, graph);
            vmap[jolt] = n;
        }

        // destination node
        long device = vmap.rbegin()->first + 3;
        auto v = boost::add_vertex(vertex_prop_t { device }, graph);
        vmap[device] = v;

        // add edges for all reachable nodes
        for (auto curr = vmap.begin(); curr != vmap.end(); curr++)
        {
            auto next = curr;
            // possibility of 3 reachable nodes from here
            for (auto i = 1; i <= 3; i++)
            {
                next++;
                if (next != vmap.end() && next->first - curr->first <= 3)
                    boost::add_edge(curr->second, next->second, graph);
                if (next == vmap.end())
                    break;
            }
        }

#if 0
        const size_t V = boost::num_vertices(graph);
        const size_t E = boost::num_edges(graph);
        std::cout << "V=" << V << " E=" << E << " u=" << u << " v=" << v << std::endl;
#endif

        // do the topo sort
        std::vector<vertex_t> sorted;
        boost::topological_sort(graph, std::back_inserter(sorted));

        // do the "dynamic" programming
        std::map<vertex_t, long> dp;
        for (auto s: sorted)
            dp[s] = 0;
        dp[v] = 1;

        for (auto t: sorted)
        {
            auto o = boost::adjacent_vertices(t, graph);
            for (auto it = o.first; it != o.second; it++)
            {
                dp[t] += dp[*it];
            }
        }
        return dp[u];
    }

    typedef std::vector<std::string> seatmap_t;
    typedef std::function<long(size_t, size_t, size_t, size_t, const seatmap_t&)> day11func_t;

    long day11worker(day11func_t occ_adj, long smax)
    {
        std::ifstream infile("../data/day11.dat");
        std::string line;

        seatmap_t seats;
        while (std::getline(infile, line))
        {
            seats.push_back(line);
        }

        auto print = [](const seatmap_t& s)
        {
            for (auto row: s)
                std::cout << row << std::endl;
            std::cout << std::endl;
        };

        //print(seats);

        // for legibility
        const size_t ROWS = seats.size();
        const size_t COLS = seats[0].size();

        while (true)
        {
            seatmap_t next;
            next.resize(ROWS, std::string(COLS, ' '));
            for (size_t row = 0; row < ROWS; row++)
            {
                for (size_t col = 0; col < COLS; col++)
                {
                    if (seats[row][col] == '.')
                        next[row][col] = '.';
                    else if (seats[row][col] == 'L')
                    {
                        if ((occ_adj)(row, col, ROWS, COLS, seats) == 0)
                            next[row][col] = '#';
                        else
                            next[row][col] = 'L';
                    }
                    else if (seats[row][col] == '#')
                    {
                        if ((occ_adj)(row, col, ROWS, COLS, seats) >= smax)
                            next[row][col] = 'L';
                        else
                            next[row][col] = '#';
                    }
                }
            }
            //print(next);
            if (next == seats)
                break;
            seats = next;
        }

        long occupied = 0;
        for (size_t row = 0; row < ROWS; row++)
            for (size_t col = 0; col < COLS; col++)
                if (seats[row][col] == '#')
                    occupied++;

        return occupied;
    }

    long day11a()
    {
        auto occ_adj = [](size_t row, size_t col, size_t ROWS, size_t COLS, const seatmap_t& lseats) -> long
        {
            long n = 0;
            for (size_t r = (row == 0 ? 0 : row - 1); r <= (row == ROWS-1 ? ROWS-1 : row+1); r++)
                for (size_t c = (col == 0 ? 0 : col - 1); c <= (col == COLS-1 ? COLS-1 : col+1); c++)
                {
                    if (r == row && c == col)
                        continue;
                    if (lseats[r][c] == '#')
                        n++;
                }
            return n;
        };
        return day11worker(occ_adj, 4);
    }

    long day11b()
    {
        auto occ_adj = [](size_t row, size_t col, size_t ROWS, size_t COLS, const seatmap_t& lseats) -> long
        {
            long n = 0;
            size_t r, c;

            // making use of the fact that taking a size_t below zero makes a big number, as long as
            // r and c are < ROWS and COLS we're in range

            // north
            r = row-1; c = col;
            while (r < ROWS && c < COLS) { if (lseats[r][c] != '.') { if (lseats[r][c] == '#') n++; break; } r--; }
            // northwest
            r = row-1; c = col-1;
            while (r < ROWS && c < COLS) { if (lseats[r][c] != '.') { if (lseats[r][c] == '#') n++; break; } r--; c--; }
            // west
            r = row; c = col-1;
            while (r < ROWS && c < COLS) { if (lseats[r][c] != '.') { if (lseats[r][c] == '#') n++; break; } c--; }
            // southwest
            r = row+1; c = col-1;
            while (r < ROWS && c < COLS) { if (lseats[r][c] != '.') { if (lseats[r][c] == '#') n++; break; } r++; c--; }
            // south
            r = row+1; c = col;
            while (r < ROWS && c < COLS) { if (lseats[r][c] != '.') { if (lseats[r][c] == '#') n++; break; } r++; }
            // southeast
            r = row+1; c = col+1;
            while (r < ROWS && c < COLS) { if (lseats[r][c] != '.') { if (lseats[r][c] == '#') n++; break; } r++; c++; }
            // east
            r = row; c = col+1;
            while (r < ROWS && c < COLS) { if (lseats[r][c] != '.') { if (lseats[r][c] == '#') n++; break; } c++; }
            // northeast
            r = row-1; c = col+1;
            while (r < ROWS && c < COLS) { if (lseats[r][c] != '.') { if (lseats[r][c] == '#') n++; break; } r--; c++; }

            return n;
        };

        return day11worker(occ_adj, 5);
    }
};
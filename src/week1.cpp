#include "week1.h"

#include <iostream>
#include <regex>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <functional>
#include <map>
#include <set>

#include <boost/algorithm/string.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>

namespace week1
{
    const int day01target = 2020;

    long day01a()
    {
        std::vector<long> input;
        std::ifstream infile("../data/day01.dat");
        std::string line;
        while (std::getline(infile, line))
            input.push_back(std::stol(line));

        // return the product of the two entries that sum to 2020. O(N^2), don't care.
        for (auto i = input.begin(); i != input.end(); i++)
        {
            for (auto j = i + 1; j != input.end(); j++)
            {
                if (*i + *j == day01target)
                {
                //std::cout << *i << " " << *j << std::endl;
                    return *i * *j;
                }
            }
        }

    // shouldn't happen
        return -1;
    }

    long day01b()
    {
        std::vector<long> input;
        std::ifstream infile("../data/day01.dat");
        std::string line;
        while (std::getline(infile, line))
            input.push_back(std::stol(line));

        // return the product of the three entries that sum to 2020. O(N^3), don't care.
        for (auto i = input.begin(); i != input.end(); i++)
        {
            for (auto j = i + 1; j != input.end(); j++)
            {
                for (auto k = j + 1; k != input.end(); k++)
                {
                   if (*i + *j + *k == day01target)
                   {
                    //std::cout << *i << " " << *j << " " << *k << std::endl;
                    return *i * *j * *k;
                    }
                }
            }
        }

        // shouldn't happen
        return -1;
    }      

    // use a lambda for the two different validation functions on day 2
    typedef std::function<bool(int, int, char, const std::string&)> day02func_t;

    long day02worker(day02func_t func)
    {
        std::vector<std::string> input;
        std::ifstream infile("../data/day02.dat");
        std::string line;
        while (std::getline(infile, line))
            input.push_back(line);

        long valid = 0;
        std::regex r("([0-9]+)-([0-9]+) ([a-z]): ([a-z]+)");
        for (auto s: input)
        {
            std::smatch m;
            std::regex_search(s, m, r);
            int lo = std::stoi(m.str(1));
            int hi = std::stoi(m.str(2));
            char ch = m.str(3)[0];
            const std::string& pw = m.str(4);

            if (func(lo, hi, ch, pw))
                valid++;
        }
        return valid; 
    }

    long day02a()
    {
        return day02worker([](int l, int h, char ch, const std::string& pw) -> bool { int num = std::count(pw.begin(), pw.end(), ch); return (num >= l && num <= h); });
    }

    long day02b()
    {
        return day02worker([](int l, int h, char ch, const std::string& pw) -> bool { return ((pw[l-1] == ch && pw[h-1] != ch) || (pw[l-1] != ch && pw[h-1] == ch)); });
    }

    long day03a(int right, int down)
    {
        std::vector<std::string> input;
        std::ifstream infile("../data/day03.dat");
        std::string line;
        while (std::getline(infile, line))
            input.push_back(line);

        size_t x = 0, y = 0;
        long trees = 0;
        while (y < input.size())
        {
        // x/y in reverse order but correct
            if (input[y][x] == '#')
                trees++;
            y += down;
            x = (x + right) % input[0].size();
        }
        return trees;
    }

    long day03b()
    {
        const std::vector<std::vector<int>> forests = { 
            { 1, 1 },
            { 3, 1 },
            { 5, 1 },
            { 7, 1 },
            { 1, 2 }
        };
        long product = 1;
        for (auto f: forests)
            product *= day03a(f[0], f[1]);
        return product;
    }

    // for day 4
    class passport
    {
    public:
        void add_kvp(const std::string& kvp);    // add a key-value pair
        bool has_fields() const;                 // does this passport have the required fields?
        bool is_valid() const;                   // does this passport have valid fields?
        std::string str() const;                 // print this password for debugging
    private:
        std::string byr, iyr, eyr, hgt, hcl, ecl, pid, cid;
    };

    // use a pointer to method function
    typedef bool (passport::*day04func_t)() const;

    void passport::add_kvp(const std::string& kvp)
    {
        std::size_t colon = kvp.find(":");
        std::string key(kvp.substr(0, colon));
        std::string value(kvp.substr(colon+1));
        if (key == "byr") byr = value;
        else if (key == "iyr") iyr = value;
        else if (key == "eyr") eyr = value;
        else if (key == "hgt") hgt = value;
        else if (key == "hcl") hcl = value;
        else if (key == "ecl") ecl = value;
        else if (key == "pid") pid = value;
        else if (key == "cid") cid = value;
        else throw std::runtime_error("bad tag");
    }

    bool passport::has_fields() const
    {
        // cid is optional
        return byr.size() && iyr.size() && eyr.size() && hgt.size() && hcl.size() && ecl.size() && pid.size();
    }

    bool passport::is_valid() const
    {
        static const std::regex height_re("([0-9]+)(cm|in)");
        static const std::regex hair_re("#[0-9a-f]{6}");
        static const std::regex eyes_re("amb|blu|brn|gry|grn|hzl|oth");
        static const std::regex pass_re("[0-9]{9}");
        std::smatch parts;
        if (!std::regex_search(hgt, parts, height_re))
            // height doesn't match regex
            return false;
        int nht = std::stoi(parts.str(1));

        return byr.size() && std::stoi(byr) >= 1920 && std::stoi(byr) <= 2002 &&
               iyr.size() && std::stoi(iyr) >= 2010 && std::stoi(iyr) <= 2020 &&
               eyr.size() && std::stoi(eyr) >= 2020 && std::stoi(eyr) <= 2030 &&
               ((parts.str(2) == "in" && nht >= 59 && nht <= 76) || (parts.str(2) == "cm" && nht >= 150 && nht <= 193)) &&
               std::regex_match(hcl, hair_re) &&
               std::regex_match(ecl, eyes_re) &&
               std::regex_match(pid, pass_re);
    }

    std::string passport::str() const
    {
        return "/" + byr + "/" + iyr + "/"+ eyr + "/" + hgt + "/" + hcl + "/" + ecl + "/" + pid + "/" + cid + " " + (is_valid() ? "Y" : "N");
    }

    long day04worker(day04func_t func)
    {
        long valid = 0;
        long total = 0;
        std::ifstream infile("../data/day04.dat"); // doesn't make sense to try to stuff this data into a structure
        std::string line;

        // this scopes awkwardly
        passport* p = new passport();
        while (std::getline(infile, line))
        {
            if (line.size() > 0)
            {
                std::istringstream iss(line);
                std::string word;
                while (iss >> word)
                    p->add_kvp(word);
            }
            else
            {
                //std::cout << p.str() << std::endl;
                if ((p->*func)())
                    valid++;
                delete p;
                p = new passport(); // get a new, empty one
                total++;
            }
        }
        // catch the last one
        if ((p->*func)())
            valid++;
        total++;
        delete p;
        return valid;
    }

    long day04a()
    {
        return day04worker(&passport::has_fields);
    }

    long day04b()
    {
        return day04worker(&passport::is_valid);
    }

    long calc_seat(const std::string& bp)
    {
        long range = 128;
        long row = 0;
        for (size_t r = 0; r < 7; r++)
        {
            range = range / 2;
            if (bp[r] == 'B')
                row += range;
        }
        range = 8;
        long seat = 0;
        for (size_t s = 7; s < 10; s++)
        {
            range = range / 2;
            if (bp[s] == 'R')
                seat += range;
        }
        return row * 8 + seat;
    }

    long day05a()
    {
        long max = 0;
        std::ifstream infile("../data/day05.dat");
        std::string line;

        while (std::getline(infile, line))
        {
            long id = calc_seat(line);
            if (id > max)
                max = id;
        }
        return max;
    }

    long day05b()
    {
        std::ifstream infile("../data/day05.dat");
        std::string line;
        std::set<long> seats;

        while (std::getline(infile, line))
        {
            seats.insert(calc_seat(line));
        }
        auto s = seats.begin();
        long prev = *s;
        while (++s != seats.end())
        {
            if (*s - prev != 1)
                return *s - 1;
            prev = *s;
        }

        // shouldn't happen
        return 0;
    }

    // for day 6, similar to day 2
    typedef std::vector<std::array<long, 26>> customs_group_t;
    typedef std::function<long(const customs_group_t&)> day06func_t;

    long day06worker(day06func_t counter)
    {
        long sum = 0;
        std::ifstream infile("../data/day06.dat");
        std::string line;
        customs_group_t group;

        // This thing where they use a blank line to delimit a group, but not after the last one
        // in the file, leaves a bit of an anti-pattern where the validation step has to be performed
        // in two different places. (Or maybe there's some other trick I don't know.) Use a lambda
        // where that code is needed, pass it into here, invoke it twice.
        while (std::getline(infile, line))
        {
            customs_group_t::value_type form = { 0 };
            if (line.size() > 0)
            {
                for (auto c: line)
                    form[c-'a']++;
                group.push_back(form);
            }
            else
            {
                sum += counter(group);
                group.clear();
            }
        }
        // get the last group, too
        sum += counter(group);
        return sum;
    }

    long day06a()
    {
        auto counter = [](const customs_group_t& group) -> long {
            long count = 0;
            for (auto i = 0; i < 26; ++i)
            {
                for (size_t p = 0; p < group.size(); p++)
                {
                    if (group[p][i])
                    {
                        count++;
                        break;
                    }
                }
            }
            return count;
        };

        return day06worker(counter);
    }

    long day06b()
    {
        auto counter = [](const customs_group_t& group) -> long {
            long questions = 0;
            for (auto i = 0; i < 26; i++)
            {
                bool q = true;
                for (size_t p = 0; p < group.size(); p++)
                {
                    q = q && group[p][i];
                }
                if (q)
                {
                    questions++;
                }
            }
            return questions;
        };

        return day06worker(counter);
    }

    // day 7
    struct bag_info { std::string name; long count; };
    struct bag_comp { bool operator()(const bag_info& l, const bag_info& r) const { return l.name < r.name; }};
    typedef std::set<bag_info, bag_comp> bagset_t;
    typedef std::map<std::string, bagset_t> bagmap_t;

    void day07load_bagmap(bagmap_t& bagmap)
    {
        std::ifstream infile("../data/day07.dat");
        std::string line;

        std::regex line_re("([a-z]+ [a-z]+) bags contain(.+)");
        std::regex contain_re(" ([0-9]+) ([a-z]+ [a-z]+) bag[s]?");

        while (std::getline(infile, line))
        {
            bagset_t inside;
            std::smatch sm1;
            std::regex_search(line, sm1, line_re);
            std::string outside = sm1.str(1); // container color
            std::string second = sm1.str(2);
            std::vector<std::string> inners;
            boost::split(inners, second, boost::is_any_of(".,"));
            for (auto inner: inners)
            {
                if (inner.size() == 0 || inner.substr(0, 3) == " no")
                {
                    break;
                }
                std::smatch sm2;
                std::regex_search(inner, sm2, contain_re);
                // 1st = number, 2nd = name
                bag_info contained = { sm2.str(2), std::stoi(sm2.str(1)) };
                inside.insert(contained);
            }
            bagmap[outside] = inside;
        }
    }

    bool day07path_exists(const bagmap_t& bagmap, const std::string& start, const std::string& finish)
    {
        if (start == finish)
            return true;

        std::map<std::string, bool> visited;
        for (auto b: bagmap)
            visited[b.first] = false;

        // queue for bfs
        std::list<std::string> queue;

        visited[start] = true;
        queue.push_back(start);

        while (!queue.empty())
        {
            std::string s = queue.front();
            queue.pop_front();
            auto b = bagmap.find(s);
            for (auto i: b->second)
            {
                if (i.name == finish)
                    return true;
                if (!visited[i.name])
                {
                    visited[i.name] = true;
                    queue.push_back(i.name);
                }
            }
        }
        return false;
    }

    long day07dfs(const bagmap_t& bagmap, const std::string& start)
    {
        long level = 0;
        auto node = bagmap.find(start);

        if (node->second.empty())
        {
            // end of the road - just me
            return 1;
        }

        for (auto n: node->second)
        {
            level += n.count * day07dfs(bagmap, n.name);
        }
        // return all the children plus myself
        return level + 1;
    }

    long day07a()
    {
        bagmap_t bagmap;
        day07load_bagmap(bagmap);

        // for each bag in the map, is there a paty to "shiny gold"?
        // surely a better way to do this than a BFS from each possible node but it finishes in under 2s
        long paths = 0;
        for (auto o: bagmap)
        {
            if (o.first == "shiny gold")
                continue;
            if (day07path_exists(bagmap, o.first, "shiny gold"))
                paths++;
        }

        return paths;
    }

    long day07b()
    {
        bagmap_t bagmap;
        day07load_bagmap(bagmap);

        // do something like a DFS from the "shiny gold" bag, counting up the inners
        return day07dfs(bagmap, "shiny gold") - 1; // don't count myself
    }

    class bfs_match_visitor : public boost::default_bfs_visitor
    {
        std::string name;
        bool& matched;
    public:
        bfs_match_visitor(const std::string& n, bool& m) : name(n), matched(m) { }
        template <typename Vertex, typename Graph>
        void discover_vertex(Vertex u, const Graph& g) const 
        {
            if (g[u].name == name)
                matched = true;
        }
    };

    long day07a_alt()
    {
        // https://stackoverflow.com/a/49232741
        struct vertex_prop_t { std::string name; boost::default_color_type color; };
        struct edge_prop_t { long count; };
        using graph_t = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, vertex_prop_t, edge_prop_t>;
        using vertex_t = boost::graph_traits<graph_t>::vertex_descriptor;
        //using edge_t = boost::graph_traits<graph_t>::edge_descriptor;

        graph_t graph;
        std::map<std::string, vertex_t> vmap;

        std::ifstream infile("../data/day07.dat");
        std::string line;
        const std::regex line_re("([a-z]+ [a-z]+) bags contain(.+)");
        const std::regex contain_re(" ([0-9]+) ([a-z]+ [a-z]+) bag[s]?");

        while (std::getline(infile, line))
        {
            std::smatch sm1;
            std::regex_search(line, sm1, line_re);
            std::string source = sm1.str(1); // outside bag color
            std::string second = sm1.str(2);
            std::vector<std::string> inners;
            boost::split(inners, second, boost::is_any_of(".,"));
            for (auto inner: inners)
            {
                if (inner.size() == 0 || inner.substr(0, 3) == " no")
                {
                    break;
                }
                std::smatch sm2;
                std::regex_search(inner, sm2, contain_re);
                long count = std::stol(sm2.str(1));
                std::string dest = sm2.str(2);

                vertex_t u;
                auto uit = vmap.find(source);
                if (uit == vmap.end())
                {
                    u = boost::add_vertex(vertex_prop_t{ source }, graph);
                    vmap.insert(std::make_pair(source, u));
                }
                else
                    u = uit->second;

                vertex_t v;
                auto vit = vmap.find(dest);
                if (vit == vmap.end())
                {
                    v = boost::add_vertex(vertex_prop_t{ dest }, graph);
                    vmap.insert(std::make_pair(dest, v));
                }
                else
                    v = vit->second;

                boost::add_edge(u, v, edge_prop_t{count}, graph);
            }
        }

        // here's the bfs to calculate whether there is a path
        long matches = 0;
        auto vp = vertices(graph);
        for (auto v = vp.first; v != vp.second; ++v)
        {
            if (graph[*v].name == "shiny gold") continue;
            bool matched = false;
            bfs_match_visitor vis("shiny gold", matched);
            boost::breadth_first_search(graph, *v, boost::color_map(boost::get(&vertex_prop_t::color, graph)).visitor(vis));
            if (matched)
                matches++;
        }

        //return boost::num_vertices(graph);
        return matches;
    }
};
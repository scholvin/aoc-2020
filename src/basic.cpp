#include "basic.h"

#include <iostream>
#include <regex>
#include <algorithm>
#include <fstream>

#include <boost/algorithm/string.hpp>
#include <boost/graph/directed_graph.hpp>

// would prefer to have these added automatically somehow
const std::map<std::string, std::function<long(void)>> basic::method_map =
{
    { "1a", &basic::day01a },
    { "1b", &basic::day01b },
    { "2a", &basic::day02a },
    { "2b", &basic::day02b },
    { "3a", std::bind(&basic::day03a, 3, 1) },
    { "3b", &basic::day03b },
    { "4a", std::bind(&basic::day04worker, &basic::passport::has_fields) },
    { "4b", std::bind(&basic::day04worker, &basic::passport::is_valid) },
    { "5a", &basic::day05a },
    { "5b", &basic::day05b },
    { "6a", &basic::day06a },
    { "6b", &basic::day06b },
    { "7a", &basic::day07a },
    { "7b", &basic::day07b },
};

void basic::run(const std::string& id)
{
    auto it = method_map.find(id);
    if (it != method_map.end())
    {
        long answer = (it->second)();
        std::cout << id << " answer: " << answer << std::endl;
    }
    else
    {
        std::cout << "id " << id << " is not recognized" << std::endl;
    }
}

void basic::run_all()
{
    for (auto it: method_map)
    {
        long answer = (it.second)();
        std::cout << it.first << " answer: " << answer << std::endl;
    }
}

std::string basic::get_keys()
{
    std::string ret = "";
    for (auto it: method_map)
    {
        ret += it.first + " ";
    }
    return ret;
}

const int day01target = 2020;

long basic::day01a()
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

long basic::day01b()
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

long basic::day02worker(day02func func)
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

long basic::day02a()
{
    return day02worker([](int l, int h, char ch, const std::string& pw) -> bool { int num = std::count(pw.begin(), pw.end(), ch); return (num >= l && num <= h); });
}

long basic::day02b()
{
    return day02worker([](int l, int h, char ch, const std::string& pw) -> bool { return ((pw[l-1] == ch && pw[h-1] != ch) || (pw[l-1] != ch && pw[h-1] == ch)); });
}

long basic::day03a(int right, int down)
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

long basic::day03b()
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

void basic::passport::add_kvp(const std::string& kvp)
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

bool basic::passport::has_fields() const
{
    // cid is optional
    return byr.size() && iyr.size() && eyr.size() && hgt.size() && hcl.size() && ecl.size() && pid.size();
}

bool basic::passport::is_valid() const
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

std::string basic::passport::str() const
{
    return "/" + byr + "/" + iyr + "/"+ eyr + "/" + hgt + "/" + hcl + "/" + ecl + "/" + pid + "/" + cid + " " + (is_valid() ? "Y" : "N");
}

long basic::day04worker(day04func func)
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

long basic::calc_seat(const std::string& bp)
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

long basic::day05a()
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

long basic::day05b()
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

long basic::day06a()
{
    auto counter = [](const customs_group& group) -> long {
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

long basic::day06b()
{
    auto counter = [](const customs_group& group) -> long {
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

long basic::day06worker(day06func counter)
{
    long sum = 0;
    std::ifstream infile("../data/day06.dat");
    std::string line;
    customs_group group;

    // This thing where they use a blank line to delimit a group, but not after the last one
    // in the file, leaves a bit of an anti-pattern where the validation step has to be performed
    // in two different places. (Or maybe there's some other trick I don't know.) Use a lambda
    // where that code is needed, pass it into here, invoke it twice.
    while (std::getline(infile, line))
    {
        customs_group::value_type form = { 0 };
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

long basic::day07a()
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

long basic::day07b()
{
    bagmap_t bagmap;
    day07load_bagmap(bagmap);

    // do something like a DFS from the "shiny gold" bag, counting up the inners
    return day07dfs(bagmap, "shiny gold") - 1; // don't count myself
}

void basic::day07load_bagmap(bagmap_t& bagmap)
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

bool basic::day07path_exists(const bagmap_t& bagmap, const std::string& start, const std::string& finish)
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

long basic::day07dfs(const bagmap_t& bagmap, const std::string& start)
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
    return level + 1;
}
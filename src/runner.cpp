#include "runner.h"

#include <iostream>
#include <iomanip>
#include <chrono>

using namespace std;
void runner::add(const method_map_t& map)
{
    for (auto it: map)
    {
        m_map.insert(it);
    }
}

void runner::run(const std::string& id) const
{
    auto it = m_map.find(id);
    if (it != m_map.end())
    {
        run_impl(id, it->second);
    }
    else
    {
        cout << "id " << id << " is not recognized" << endl;
    }
}

void runner::run_all() const
{
    auto start = chrono::steady_clock::now();
    for (auto it: m_map)
    {
        run_impl(it.first, it.second);
    }
    auto end = chrono::steady_clock::now();
    std::cout << setw(34) << chrono::duration_cast<chrono::microseconds>(end - start).count() << "µs"
              << endl;   
}

std::string runner::get_keys() const
{
    string ret = "";
    for (auto it: m_map)
    {
        ret += it.first + " ";
    }
    return ret;
}

void runner::run_impl(const string& id, basic_func_t func) const
{
    auto start = chrono::steady_clock::now();
    long answer = (func)();
    auto end = chrono::steady_clock::now();
    std::cout << setw(6) << id 
              << " answer:" << setw(12) << answer 
              << setw(8) << chrono::duration_cast<chrono::microseconds>(end - start).count() << "µs"
              << endl;   
}
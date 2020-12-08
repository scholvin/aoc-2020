#include "runner.h"

#include <iostream>

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
        long answer = (it->second)();
        std::cout << id << " answer: " << answer << std::endl;
    }
    else
    {
        std::cout << "id " << id << " is not recognized" << std::endl;
    }
}

void runner::run_all() const
{
    for (auto it: m_map)
    {
        long answer = (it.second)();
        std::cout << it.first << " answer: " << answer << std::endl;
    }
}

std::string runner::get_keys() const
{
    std::string ret = "";
    for (auto it: m_map)
    {
        ret += it.first + " ";
    }
    return ret;
}

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

const int ANSWER_WIDTH = 16;
const int ID_WIDTH = 7;
const int MICROS_WIDTH = 12;

void runner::run(int argc, char** argv)
{
    auto start = chrono::steady_clock::now();
    if (string(argv[1]) == "all")
    {
        for (auto it: m_map)
        {
            run_impl(it.first, it.second);
        }
    }
    else
    {
        for (int i = 1; i < argc; ++i)
        {
            auto it = m_map.find(argv[i]);
            if (it != m_map.end())
            {
                run_impl(it->first, it->second);
            }
            else
            {
                cout << "id " << argv[i] << " is not recognized" << endl;
            }
        }
    }
    if (m_count > 1)
    {
        auto end = chrono::steady_clock::now();
        auto micros = chrono::duration_cast<chrono::microseconds>(end - start).count();
        cout << "TOTAL:"
             << setw(ID_WIDTH + ANSWER_WIDTH + MICROS_WIDTH + 2) << micros << "µs"
             << " (" << setprecision(4) << micros / 1000000.0 << "s)" << endl;
    }
}

string runner::get_keys() const
{
    string ret = "";
    for (auto it: m_map)
    {
        ret += it.first + " ";
    }
    return ret;
}

void runner::run_impl(const string& id, basic_func_t func)
{
    auto start = chrono::steady_clock::now();
    long answer = (func)();
    auto end = chrono::steady_clock::now();
    cout << setw(ID_WIDTH) << id
         << " answer:" << setw(ANSWER_WIDTH) << answer
         << setw(MICROS_WIDTH) << chrono::duration_cast<chrono::microseconds>(end - start).count() << "µs"
         << endl;
    m_count++;
}

bool runner::new_order::operator()(const string& left, const string& right) const
{
    // degenerate case: someone passed in an invalid string
    if ((left.size() && (left[0] < '0' || left[0] > '9')) ||
        (right.size() && (right[0] < '0' || right[0] > '9')))
        return left < right;

    // idea is to make 10a come after 9b - this is serious overkill
    auto lindex = left.find_first_not_of("0123456789");
    if (lindex == std::string::npos)
        return left < right;
    int lnum = stoi(left.substr(0, lindex));
    string lstr = left.substr(lindex);
    auto rindex = right.find_first_not_of("0123456789");
    if (rindex == std::string::npos)
        return left < right;
    int rnum = stoi(right.substr(0, rindex));
    string rstr = right.substr(rindex);

    if (lnum == rnum)
        return lstr < rstr;
    return lnum < rnum;
}

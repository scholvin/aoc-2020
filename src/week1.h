#pragma once

#include "runner.h"

namespace week1
{
	long day01a();
    long day01b();
    long day02a();
    long day02b();
    long day03a(int right, int down);
    long day03b();
    long day04a();
    long day04b();
    long day05a();
    long day05b();
    long day06a();
    long day06b();
    long day07a();
    long day07b();
    long day07a_alt();

	const runner::method_map_t method_map =
	{
        { "1a", &day01a },
        { "1b", &day01b },
        { "2a", &day02a },
        { "2b", &day02b },
        { "3a", std::bind(&day03a, 3, 1) },
        { "3b", &day03b },
        { "4a", &day04a },
        { "4b", &day04b },
        { "5a", &day05a },
        { "5b", &day05b },
        { "6a", &day06a },
        { "6b", &day06b },
        { "7a", &day07a },
        { "7b", &day07b },
        { "7a_alt", &day07a_alt },
    };
};
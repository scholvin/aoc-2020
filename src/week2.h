#pragma once

#include "runner.h"

namespace week2
{
    long day08a();
    long day08b();
    long day09a();
    long day09b();
    long day10a();
    long day10b();
    long day11a();
    long day11b();
    long day12a();
    long day12b();

	const runner::method_map_t method_map =
	{
        { "8a", &day08a },
        { "8b", &day08b },
        { "9a", &day09a },
        { "9b", &day09b },
        { "10a", &day10a },
        { "10b", &day10b },
        { "11a", &day11a },
        { "11b", &day11b },
        { "12a", &day12a },
        { "12b", &day12b },
    };
};
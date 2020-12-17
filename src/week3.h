#pragma once

#include "runner.h"

namespace week3
{
    long day15(size_t turns);
    long day16a();
    long day16b();
	long day17a();
	
	const runner::method_map_t method_map =
	{
        { "15a", std::bind(&day15, 2020) },
        { "15b", std::bind(&day15, 30000000) },
        { "16a", &day16a },
        { "16b", &day16b },
        { "17a", &day17a },
    };
};

#pragma once

#include "runner.h"

namespace week3
{
    long day15(size_t turns);

	const runner::method_map_t method_map =
	{
        { "15a", std::bind(&day15, 2020) },
        { "15b", std::bind(&day15, 30000000) },
    };
};

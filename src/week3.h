#pragma once

#include "runner.h"

namespace week3
{
    long day15a();
    long day15b();

	const runner::method_map_t method_map =
	{
        { "15a", &day15a },
        { "15b", &day15b },
    };
};

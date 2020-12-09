#pragma once

#include "runner.h"

namespace week2
{
    long day08a();
    long day08b();
    long day09a();
    long day09b();

	const runner::method_map_t method_map =
	{
        { "8a", &day08a },
        { "8b", &day08b },
        { "9a", &day09a },
        { "9b", &day09b },
    };
};
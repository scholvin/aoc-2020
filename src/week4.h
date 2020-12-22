#pragma once

#include "runner.h"

namespace week4
{
    long day22a();
    long day22b();

    const runner::method_map_t method_map =
    {
        { "22a", &day22a },
        { "22b", &day22b },
    };
};

#pragma once

#include "runner.h"

namespace week4
{
    long day22a();

    const runner::method_map_t method_map =
    {
        { "22a", &day22a },
    };
};

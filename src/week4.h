#pragma once

#include "runner.h"

namespace week4
{
    long day22a();
    long day22b();
    long day23a();
    long day23b();
    long day24a();
    long day24b();
    long day25a();

    const runner::method_map_t method_map =
    {
        { "22a", &day22a },
        { "22b", &day22b },
        { "23a", &day23a },
        { "23b", &day23b },
        { "24a", &day24a },
        { "24b", &day24b },
        { "25a", &day25a },
    };
};

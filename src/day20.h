#pragma once

#include "runner.h"

namespace day20
{
    long day20a();
    long day20b();

    const runner::method_map_t method_map =
    {
        { "20a", &day20a },
        { "20b", &day20b },
    };
};

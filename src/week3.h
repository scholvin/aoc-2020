#pragma once

#include "runner.h"

namespace week3
{
    long day15(size_t turns);
    long day16a();
    long day16b();
    long day17a();
    long day17b();
    long day17a_alt();
    long day18(bool equal_precedence);
    long day19a();
    long day19b();
    long day21();

    const runner::method_map_t method_map =
    {
        { "15a", std::bind(&day15, 2020) },
        { "15b", std::bind(&day15, 30000000) },
        { "16a", &day16a },
        { "16b", &day16b },
        { "17a", &day17a },
        { "17b", &day17b },
        { "17a_alt", &day17a_alt },
        { "18a", std::bind(&day18, true) },
        { "18b", std::bind(&day18, false) },
        { "19a", &day19a },
        { "19b", &day19b },
        // see day20.h
        { "21ab", &day21 },
    };
};

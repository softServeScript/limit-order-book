#pragma once
#include <cstdint>
#include <vector>

// Includes only static stats
// Benchmarks stored in random_order_generator.cpp

struct StressStats {
    int tt_ = 0;        // total trades
    int tpr_ = 0;       // trades per runs
    int at_ = 0;        // active trades
    double fill_ratio_ = 0.0;
    int cancelled_orders_ = 0;
    int total_bought_ = 0;
    int total_sold_ = 0;
};

struct BMStats {
    int opps_ = 0; // orders processed per second (ORDERS/TIME)
    double p50_ = 0;
    double p99_ = 0;
    int max_ = 0;
};

struct OPSStats {
    size_t count_ = 0;
    size_t current_ = 0;

    std::vector<size_t> vals_;


    size_t high_ = 0;
    size_t low_ = 0;

    size_t mean_ = 0.0;
};
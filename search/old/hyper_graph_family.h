#pragma once

#include <vector>
#include <cstdint>
#include <random>
#include "hyper_graph.h"

class HyperGraphFamily {

public:
    std::vector <uint32_t> edge_sizes_;
    std::vector <uint32_t> edge_weights_;
    bool is_coupled_;
    HyperGraphFamily(const std::vector <uint32_t>& edge_sizes, const std::vector <uint32_t>& edge_weights, bool is_coupled = false);

    HyperGraph sample(uint32_t edge_count, double load_factor, std::mt19937 rng) const;

    double threshold(uint32_t edge_count, std::mt19937 rng, double left = 0.0, double right = 1.0, double epsilon = 0.0001) const;
    double average_edge_size() const;
};

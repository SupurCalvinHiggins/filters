#pragma once

#include <vector>
#include <cstdint>
#include <random>
#include "hyper_graph.h"

class HyperGraphFamily {

public:
    std::vector <uint32_t> edge_sizes_;
    std::vector <uint32_t> edge_weights_;
    uint32_t window_size_;
    HyperGraphFamily(const std::vector <uint32_t>& edge_sizes, const std::vector <uint32_t>& edge_weights, uint32_t window_size = 0);

    HyperGraph sample(uint32_t edge_count, double load_factor, std::mt19937 rng) const;

    double threshold(uint32_t edge_count, double left = 0.0, double right = 1.0, double epsilon = 0.0001, std::mt19937 rng) const;
    double average_edge_size() const;
};

#pragma once

#include <vector>
#include <cstdint>

#include "hyper_graph.h"

class HyperGraphFamily {

public:
    std::vector <uint32_t> edge_sizes_;
    std::vector <uint32_t> edge_weights_;
    uint32_t window_size_;
    HyperGraphFamily(std::vector <uint32_t> &&edge_sizes, std::vector <uint32_t> &&edge_weights, uint32_t window_size = 0);

    HyperGraph sample(uint32_t edge_count, double load_factor);

    double threshold(uint32_t edge_count, double left = 0.0, double right = 1.0, double epsilon = 0.0001);
    double average_edge_size() const;
};

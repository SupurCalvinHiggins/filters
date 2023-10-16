#pragma once

#include <vector>
#include <cstdint>

#include "hyper_graph.h"

class RandomHyperGraphFamily {
private:
    std::vector<uint32_t> edge_sizes_;
    std::vector<uint32_t> edge_weights_;

public:
    RandomHyperGraphFamily(std::vector<uint32_t> &&edge_sizes, std::vector<uint32_t> &&edge_weights);
    HyperGraph sample(uint32_t edge_count, double load_factor);
};
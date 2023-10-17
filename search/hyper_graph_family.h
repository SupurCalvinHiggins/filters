#pragma once

#include <vector>
#include <cstdint>

#include "hyper_graph.h"

class HyperGraphFamily {
private:
    std::vector <uint32_t> edge_sizes_;
    std::vector <uint32_t> edge_weights_;

public:
    HyperGraphFamily(std::vector <uint32_t> &&edge_sizes, std::vector <uint32_t> &&edge_weights);

    HyperGraph sample(uint32_t edge_count, double load_factor);

    float binary_search(double epsilon);
};

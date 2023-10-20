#include <iostream>
#include <random>
#include <cassert>

#include "hyper_graph.h"
#include "hyper_graph_family.h"
#include "message_queue.h"

// need a function that yields all combinations of edges up to threshold
// need a function that yields all combinations of probs
// need to implement coupling
// need to seperate threshold from hypergraphfamily and make into template

HyperGraphFamily random_hyper_graph_family(double max_average_edge_size, uint32_t max_edge_size, uint32_t max_edge_count, uint32_t max_weight, uint32_t seed) {
    assert(max_edge_size >= 3);
    assert(max_average_edge_size >= 3.0);
    assert(max_edge_count >= 1);

    std::mt19937 rng(seed);
    std::uniform_int_distribution<uint32_t> edge_size_dist(3, max_edge_size);
    std::uniform_int_distribution<uint32_t> edge_count_dist(1, max_edge_count);

    while (1) {
        auto edge_size = edge_size_dist(rng);

        std::vector<uint32_t> edge_sizes;
        for (uint32_t i = 0; i < edge_size; ++i)
            edge_sizes.push_back(edge_count_dist(rng));
            
        std::vector<uint32_t> edge_weights;
        for (uint32_t i = 0; i < edge_size; ++i)
            edge_weights.push_back(edge_size_dist(rng));

        HyperGraphFamily f(std::move(edge_sizes), std::move(edge_weights));

        if (f.average_edge_size() < max_average_edge_size)
            return f;
    }
}

int main() {
    // terrible code that doesnt really work at all :)
    auto f = random_hyper_graph_family(5.0, 30, 7, 10, 42);
}

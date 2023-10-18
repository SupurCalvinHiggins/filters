#include <cassert>
#include <random>
#include "hyper_graph_family.h"
#include <algorithm>

HyperGraphFamily::HyperGraphFamily(
    std::vector<uint32_t> &&edge_sizes,
    std::vector<uint32_t> &&edge_weights
) : edge_sizes_(edge_sizes), edge_weights_(edge_weights) {
    assert(edge_sizes_.size() > 0);
    assert(edge_weights_.size() > 0);
    assert(edge_sizes_.size() == edge_weights_.size());
}

HyperGraph HyperGraphFamily::sample(uint32_t edge_count, double load_factor)
{
    assert(edge_count > 0);
    assert(load_factor < 1);

    // Set up the data for the hypergraph.
    const uint32_t vertex_count = std::ceil(edge_count * (1 / load_factor));
    std::vector<std::vector<uint32_t>> edges(edge_count);

    // Set up random number generation.
    std::mt19937 rng(0);
    std::discrete_distribution<uint32_t> edge_sizes_idx_dist(edge_weights_.begin(), edge_weights_.end());
    std::uniform_int_distribution<uint32_t> vertex_dist(0, vertex_count - 1);

    // Build the hypergraph edges.
    for (auto &edge : edges)
    {
        const auto edge_size = edge_sizes_[edge_sizes_idx_dist(rng)];
        edge.resize(edge_size);
        for (auto &vertex : edge)
            vertex = vertex_dist(rng);
    }

    return HyperGraph(vertex_count, std::move(edges));
}

// epsilon = 1e-6 = 0.000006 etc.
float HyperGraphFamily::threshold(uint32_t edge_count, double left, double right, double epsilon) {

    // floating point precision fix
    while (right-left > epsilon) {
        float mid = left + (right - left) / 2;

        // if no we can move left up
        if (sample(edge_count, mid).is_core_empty()) {
            left = mid;
        }
        // otherwise if yes we can move right down
        else {
            right = mid;
        }
    }

    //return the midpoint, left or right doesn't matter
    return left;
}

#include <cassert>
#include <random>
#include "hyper_graph_family.h"
#include <algorithm>
#include <iostream>

HyperGraphFamily::HyperGraphFamily(
    const std::vector<uint32_t>& edge_sizes,
    const std::vector<uint32_t>& edge_weights,
    uint32_t window_size
) : edge_sizes_(edge_sizes), edge_weights_(edge_weights), window_size_(window_size) {
    assert(edge_sizes_.size() > 0);
    assert(edge_weights_.size() > 0);
    assert(edge_sizes_.size() == edge_weights_.size());
}

HyperGraph HyperGraphFamily::sample(uint32_t edge_count, double load_factor) const
{
    assert(edge_count > 0);
    assert(load_factor < 1);

    // Set up the data for the hypergraph.
    const uint32_t vertex_count = std::ceil(edge_count * (1 / load_factor));
    std::vector<std::vector<uint32_t>> edges(edge_count);

    // No coupling.
    if (window_size_ == 0) {

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

    // With coupling.
    } else {
        // Set up random number generation.
        std::mt19937 rng(0);
        std::discrete_distribution<uint32_t> edge_sizes_idx_dist(edge_weights_.begin(), edge_weights_.end());
        std::uniform_int_distribution<uint32_t> window_idx_dist(0, vertex_count - 1 - window_size_);

        std::vector<uint32_t> offsets;
        for (uint32_t i = 0; i < window_size_; ++i)
            offsets.push_back(i);

        // Build the hypergraph edges.
        for (auto &edge : edges)
        {
            const auto edge_size = edge_sizes_[edge_sizes_idx_dist(rng)];
            edge.resize(edge_size);
            const auto window_idx = window_idx_dist(rng);
            std::shuffle(offsets.begin(), offsets.end(), rng);
            for (uint32_t i = 0; i < edge.size(); ++i) {
                edge[i] = window_idx + offsets[i];
            }
        }
    }
    return HyperGraph(vertex_count, edges);
}

// epsilon = 1e-6 = 0.000006 etc.
double HyperGraphFamily::threshold(uint32_t edge_count, double left, double right, double epsilon) const {

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

double HyperGraphFamily::average_edge_size() const {
    uint32_t edge_weighed_size_sum = 0;
    for (uint32_t i = 0; i < edge_sizes_.size(); ++i)
        edge_weighed_size_sum += edge_sizes_[i] * edge_weights_[i];
    
    uint32_t edge_weight_sum = 0;
    for (const auto edge_weight : edge_weights_)
        edge_weight_sum += edge_weight;

    return static_cast<double>(edge_weighed_size_sum) / static_cast<double>(edge_weight_sum);
}

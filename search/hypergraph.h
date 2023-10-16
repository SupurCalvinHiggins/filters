#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
#include <random>
#include <ranges>
#include <cassert>
#include <optional>

class HyperGraph
{
private:
    std::vector<std::vector<uint32_t>> vertex_to_edges_;
    std::vector<std::vector<uint32_t>> edge_to_vertices_;
    std::optional<bool> is_core_empty_;

public:

    /*
     * Construct a hypergraph from a list of edges and the number of vertices.
     */
    HyperGraph(
        const uint32_t vertex_count,
        std::vector<std::vector<uint32_t>>&& edges
    ) : edge_to_vertices_(edges), is_core_empty_(std::nullopt) {
        // Ensure the input is valid..
        assert(vertex_count > 0);
        assert(edges.size() > 0);

        // Map the vertices to edges.
        vertex_to_edges_.resize(vertex_count);
        for (uint32_t i = 0; i < edge_to_vertices_.size(); ++i) {
            const auto& edge = edge_to_vertices_[i];
            for (const auto v : edge) {
                // Ensure each vertex is valid.
                assert(v < vertex_count);
                vertex_to_edges_[v].push_back(i);
            }
        }
    }

    bool is_core_empty()
    {
        if (is_core_empty_.has_value())
            return is_core_empty_.value();
        
        std::queue<uint32_t> q;
        for (uint32_t v = 0; v < vertex_to_edges_.size(); ++v)
        {
            const auto& edges = vertex_to_edges_[v];
            if (edges.size() == 1)
                q.push(v);
        }

        uint32_t peeled_edges = 0;
        while (!q.empty()) {
            const auto v = q.front();
            q.pop();
            const auto& edges = vertex_to_edges_[v];

            if (edges.size() != 1) continue;
            ++peeled_edges;
            const auto e = edges[0];
            const auto& vertices = edge_to_vertices_[e];
            for (const auto v : vertices) {
                auto& edges = vertex_to_edges_[v];
                edges.erase(std::remove(edges.begin(), edges.end(), e), edges.end());
                if (edges.size() == 1) {
                    q.push(v);
                }
            }   
        }
        
        is_core_empty_ = std::make_optional(peeled_edges == edge_to_vertices_.size());
        return is_core_empty_.value();
    }
};

class RandomHyperGraphFamily {
private:
    std::vector<uint32_t> edge_sizes_;
    std::vector<uint32_t> edge_weights_;

public:
    /*
     * Construct a family of random hypergraphs from a list of edge sizes (the number
     * of vertices in each edge) and the relative weights of each size. For example,
     * if the edge sizes are {3, 21} and the edge weights are {89, 11}, the hypergraph
     * family will produce graphs with 89% size 3 and 11% size 21 hyperedges.
     */
    RandomHyperGraphFamily(
        std::vector<uint32_t>&& edge_sizes,
        std::vector<uint32_t>&& edge_weights
    ) : edge_sizes_(edge_sizes), edge_weights_(edge_weights) {
        assert(edge_sizes_.size() > 0);
        assert(edge_weights_.size() > 0);
        assert(edge_sizes_.size() == edge_weights_.size());
    }

    /*
     * Return a random hypergraph sampled from the family of random hypergraphs with
     * a particular number of edges. The load factor (alpha) is the ratio between
     * the number of vertices and the number of edges.
     */
    HyperGraph sample(const uint32_t edge_count, const double load_factor) {
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
        for (auto& edge : edges) {
            const auto edge_size = edge_sizes_[edge_sizes_idx_dist(rng)];
            edge.resize(edge_size);
            for (auto& vertex : edge)
                vertex = vertex_dist(rng);
        }

        return HyperGraph(vertex_count, std::move(edges));
    }
};
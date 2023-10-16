#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
#include <random>
#include <ranges>


class HyperGraph
{
private:
    std::vector<std::vector<uint32_t>> vertex_to_edges_;
    std::vector<std::vector<uint32_t>> edge_to_vertices_;
    bool invalid_;
    bool peelable_;

public:
    HyperGraph(const uint32_t edge_size, const uint32_t edge_count, const double load_factor, std::default_random_engine& rng) : invalid_(false), peelable_(false)
    {
        const uint32_t vertex_count = std::ceil(edge_count * (1 / load_factor));
        
        std::uniform_int_distribution<uint32_t> dist(0, vertex_count - 1);
        edge_to_vertices_.reserve(edge_count);
        for (uint32_t i = 0; i < edge_count; ++i) {
            std::vector<uint32_t> edge;
            edge.resize(edge_size);
            std::ranges::sample(std::views::iota(0u, vertex_count - 1), edge.begin(), edge_size, rng);
            edge_to_vertices_.push_back(edge);
        }

        vertex_to_edges_.resize(vertex_count);
        for (uint32_t i = 0; i < edge_count; ++i) {
            const auto& edge = edge_to_vertices_[i];
            for (const auto v : edge)
                vertex_to_edges_[v].push_back(i);
        }
    }

    bool peelable()
    {
        if (!invalid_) {
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
            invalid_ = true;
            peelable_ = peeled_edges == edge_to_vertices_.size();
        }

        return peelable_;
    }
};


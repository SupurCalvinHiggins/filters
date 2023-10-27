#include <queue>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <iostream>

#include "hyper_graph.h"

/*
 * Construct a hypergraph from a list of edges and the number of vertices.
 */
HyperGraph::HyperGraph(
    uint32_t vertex_count,
    const std::vector<std::vector<uint32_t>>& edges) : vertex_to_edges_(vertex_count), edge_to_vertices_(edges), is_core_empty_(std::nullopt)
{
    // Ensure the input is valid.
    assert(vertex_to_edges_.size() > 0);
    assert(edge_to_vertices_.size() > 0);

    // Map the vertices to edges.
    for (uint32_t i = 0; i < edge_to_vertices_.size(); ++i)
    {
        const auto &edge = edge_to_vertices_[i];
        for (const auto v : edge)
        {
            // Ensure each vertex is valid.
            assert(v < vertex_count);

            // Ignore duplicates.
            auto& edges = vertex_to_edges_[v];
            if (std::find(edges.begin(), edges.end(), i) == edges.end())
                edges.push_back(i);
        }
    }
}

bool HyperGraph::is_core_empty()
{
    if (is_core_empty_.has_value())
        return is_core_empty_.value();

    std::queue<uint32_t> q;
    for (uint32_t v = 0; v < vertex_to_edges_.size(); ++v)
    {
        const auto &edges = vertex_to_edges_[v];
        if (edges.size() == 1)
            q.push(v);
    }

    uint32_t peeled_edges = 0;
    while (!q.empty())
    {
        const auto v = q.front();
        q.pop();
        const auto &edges = vertex_to_edges_[v];

        if (edges.size() != 1)
            continue;
        ++peeled_edges;
        const auto e = edges[0];
        const auto &vertices = edge_to_vertices_[e];
        for (const auto v : vertices)
        {
            auto &edges = vertex_to_edges_[v];
            edges.erase(std::remove(edges.begin(), edges.end(), e), edges.end());
            if (edges.size() == 1)
            {
                q.push(v);
            }
        }
    }

    is_core_empty_ = std::make_optional(peeled_edges == edge_to_vertices_.size());
    return is_core_empty_.value();
}
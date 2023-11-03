#pragma once

#include <vector>
#include <optional>
#include <cstdint>

class HyperGraph
{
private:
    std::vector<std::vector<uint32_t>> vertex_to_edges_;
    std::vector<std::vector<uint32_t>> edge_to_vertices_;
    std::optional<bool> is_core_empty_;

public:

    HyperGraph(uint32_t vertex_count, const std::vector<std::vector<uint32_t>>& edges);
    bool is_core_empty();
};
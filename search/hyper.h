#include <random>
#include <cassert>
#include <optional>
#include <queue>

class HyperGraph {
private:
    std::vector<std::vector<uint32_t>> m_vertex_to_edges;
    std::vector<std::vector<uint32_t>> m_edge_to_vertices;
    std::optional<bool> m_is_core_empty;

public:
    /**
     * @brief Construct a new Hyper Graph object
     * 
     * @param edge_to_vertices 
     * @param max_vertex 
     */
    HyperGraph(
        uint32_t max_vertex,
        std::vector<std::vector<uint32_t>>&& edge_to_vertices
    ) : m_edge_to_vertices(edge_to_vertices), m_is_core_empty(std::nullopt) {

        assert(m_edge_to_vertices.size() > 0);
        assert(max_vertex > 0);

        // Map each vertex to the set of all edges that contain it. 
        m_vertex_to_edges.resize(max_vertex + 1);
        for (uint32_t i = 0; i < m_edge_to_vertices.size(); ++i) {
            for (const auto v : m_edge_to_vertices[i]) {
                assert(v <= max_vertex);

                // Only add the edge if it hasn't already been added.
                if (
                    std::find(
                        m_vertex_to_edges[v].begin(), 
                        m_vertex_to_edges[v].end(), 
                        i
                    ) == m_vertex_to_edges[v].end()
                ) {
                    m_vertex_to_edges[v].push_back(i);
                }
            }
        }
    }

    /**
     * @brief 
     * 
     * @return true 
     * @return false 
     */
    bool is_core_empty() {
        // If there is a cached result, return it.
        if (m_is_core_empty.has_value())
            return m_is_core_empty.value();

        // Form queue of vertices that have peelable edges. 
        std::queue<uint32_t> q;
        for (uint32_t v = 0; v < m_vertex_to_edges.size(); ++v) {
            if (m_vertex_to_edges[v].size() == 1) {
                q.push(v);
            }
        }
        uint32_t peeled_count = 0;

        while (!q.empty()){
            // If the first vertex in the queue does not have a peelable edge, ignore it.
            const auto v = q.front();
            q.pop();
            if (m_vertex_to_edges[v].size() != 1)
                continue;
            
            // Otherwise, peel the edge.
            ++peeled_count;
            const auto e = m_vertex_to_edges[v][0];

            for (uint32_t i = 0; i < m_edge_to_vertices[e].size(); ++i) {
                // Remove the edge from the vertex to edge mapping.
                const auto v = m_edge_to_vertices[e][i];
                m_vertex_to_edges[v].erase(
                    std::remove(
                        m_vertex_to_edges[v].begin(), 
                        m_vertex_to_edges[v].end(), 
                        e
                    ), 
                    m_vertex_to_edges[v].end()
                );

                // If the vertex is now part of a peelable edge, add it to the queue.
                if (m_vertex_to_edges[v].size() == 1) {
                    q.push(v);
                }
            }
        }

        // Cache the return value.
        m_is_core_empty = std::make_optional(peeled_count == m_edge_to_vertices.size());
        return m_is_core_empty.value();
    }
};

class HyperGraphFamily {
public:
    virtual HyperGraph sample(double load_factor, std::mt19937& rng) const;
};

class CoupledHyperGraphFamily {
public:
    const uint32_t edge_count = 1000000;

public:
    const std::vector<uint32_t> edge_sizes;
    const std::vector<uint32_t> edge_weights;

public:
    CoupledHyperGraphFamily(
        std::vector<uint32_t>&& edge_sizes,
        std::vector<uint32_t>&& edge_weights
    ) : edge_sizes(edge_sizes), edge_weights(edge_weights) {
        assert(edge_sizes.size() > 0);
        assert(edge_weights.size() > 0);
        assert(edge_sizes.size() == edge_weights.size());
    }

    HyperGraph sample(double load_factor, std::mt19937& rng) const {
        assert(load_factor > 0.0);
        assert(load_factor < 1.0);

        // Allocate space for the edges.
        const uint32_t vertex_count = std::ceil(edge_count * (1 / load_factor));
        std::vector<std::vector<uint32_t>> edges(edge_count);

        // Formula from binary fuse filters. () 
        size_t window_size = 1L << static_cast<size_t>(floor(log(vertex_count) / log(3.33) + 2.25));

        std::discrete_distribution<uint32_t> edge_sizes_idx_dist(edge_weights.begin(), edge_weights.end());
        std::uniform_int_distribution<uint32_t> window_start_dist(0, vertex_count - window_size);
        std::uniform_int_distribution<uint32_t> window_offset_dist(0, window_size - 1);

        // Build the hypergraph edges.
        for (auto &edge : edges)
        {
            const auto edge_size = edge_sizes[edge_sizes_idx_dist(rng)];
            edge.resize(edge_size);

            const auto window_start = window_start_dist(rng);
            for (uint32_t i = 0; i < edge.size(); ++i) {
                edge[i] = window_start + window_offset_dist(rng);
            }
        }

        return HyperGraph(vertex_count - 1, std::move(edges));
    }

    double average_edge_size() const {
        uint32_t edge_weighed_size_sum = 0;
        for (uint32_t i = 0; i < edge_sizes.size(); ++i)
            edge_weighed_size_sum += edge_sizes[i] * edge_weights[i];
        
        uint32_t edge_weight_sum = 0;
        for (const auto edge_weight : edge_weights)
            edge_weight_sum += edge_weight;

        return static_cast<double>(edge_weighed_size_sum) / static_cast<double>(edge_weight_sum);
    }

};

double is_core_empty_threshold(const CoupledHyperGraphFamily& family, std::mt19937& rng, double low = 0.0, double high = 1.0) {
    assert(low >= 0.0);
    assert(high <= 1.0);
    assert(low <= high);

    // Error tolerance for computed threshold.
    const double eps = 0.001;

    // Binary search for the threshold.
    while (high - low > eps) {
        float mid = low + (high - low) / 2;
        int empty_count = 0;
        for (int i = 0; i < 3; ++i) {
            empty_count += family.sample(mid, rng).is_core_empty();
        }
        if (empty_count >= 2)  low = mid;
        else                   high = mid;
    }

    return low;
}

CoupledHyperGraphFamily random_family(double max_average_edge_size, uint32_t max_edge_count, uint32_t max_edge_size, uint32_t max_weight, std::mt19937& rng) {
    assert(max_average_edge_size >= 3.0);
    assert(max_edge_count >= 2);
    assert(max_edge_size >= 3);

    std::uniform_int_distribution<uint32_t> edge_count_dist(2, max_edge_count);
    std::uniform_int_distribution<uint32_t> edge_size_dist(3, max_edge_size);
    std::uniform_int_distribution<uint32_t> edge_weight_dist(1, max_weight);

    while (1) {
        const auto edge_count = edge_count_dist(rng);

        std::vector<uint32_t> edge_sizes;
        for (uint32_t i = 0; i < edge_count; ++i)
            edge_sizes.push_back(edge_size_dist(rng));

        std::vector<uint32_t> edge_weights;
        for (uint32_t i = 0; i < edge_count; ++i)
            edge_weights.push_back(edge_weight_dist(rng));

        CoupledHyperGraphFamily family(std::move(edge_sizes), std::move(edge_weights));

        if (family.average_edge_size() - 3.0 < 0.000001)
            continue;

        if (family.average_edge_size() < max_average_edge_size)
            return family;
    }
}
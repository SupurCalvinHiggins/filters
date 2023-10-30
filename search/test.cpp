#include <iostream>
#include <cassert>
#include <cmath>
#include <vector>

#include "hyper_graph.h"
#include "hyper_graph_family.h"
#include "message_queue.h"

void test_hyper_graph() {
    HyperGraph g1(5, {{0, 0, 4}, {4, 1, 1}});
    assert(g1.is_core_empty());

    HyperGraph g2(5, {{0, 0, 1}, {0, 1, 1}});
    assert(!g2.is_core_empty());
}

void test_hyper_graph_family() {
    const uint32_t edge_count = 100000;
    std::mt19937 rng(0);

    HyperGraphFamily f1({3}, {1});
    assert(f1.sample(edge_count, 0.81,rng).is_core_empty());
    assert(!f1.sample(edge_count, 0.83,rng).is_core_empty());

    HyperGraphFamily f2({4}, {1});
    assert(f2.sample(edge_count, 0.76,rng).is_core_empty());
    assert(!f2.sample(edge_count, 0.78,rng).is_core_empty());

    HyperGraphFamily f3({5}, {1});
    assert(f3.sample(edge_count, 0.69,rng).is_core_empty());
    assert(!f3.sample(edge_count, 0.71,rng).is_core_empty());

    HyperGraphFamily f4({3, 21}, {89, 11});
    assert(f4.sample(edge_count, 0.91,rng).is_core_empty());
    assert(!f4.sample(edge_count, 0.93,rng).is_core_empty());

    HyperGraphFamily f5({3}, {1}, 3);
    std::cout << f5.sample(27, 0.9,rng).is_core_empty() << std::endl;
    assert(f5.sample(edge_count, 0.91,rng).is_core_empty());
    assert(!f5.sample(edge_count, 0.93,rng).is_core_empty());
}

void test_threshold() {
    const uint32_t edge_count = 100000;
    std::mt19937 rng(0);

    HyperGraphFamily f1({3}, {1});
    assert(f1.threshold(edge_count,0.0,1.0, 0.0001,rng) > 0.81);
    assert(f1.threshold(edge_count) < 0.83);


    HyperGraphFamily f2({4}, {1});
    assert(f2.threshold(edge_count, 0.0, 1.0, 0.0001,rng) > 0.76);
    assert(f2.threshold(edge_count, 0.0,1.0, 0.0001,rng) < 0.78);


    HyperGraphFamily f3({5}, {1});
    assert(f3.threshold(edge_count,0.0,1.0, 0.0001,rng) > 0.69);
    assert(f3.threshold(edge_count,0.0,1.0, 0.0001,rng) < 0.71);

    HyperGraphFamily f4({3, 21}, {89, 11});
    assert(f4.threshold(edge_count,0.0,1.0, 0.0001,rng) > 0.91);
    assert(f4.threshold(edge_count,0.0,1.0, 0.0001,rng) < 0.93);
}

void test_average_edge_size() {
    HyperGraphFamily f1({3}, {1});
    assert(f1.average_edge_size() - 3.0 < 0.0001);

    HyperGraphFamily f2({3}, {4});
    assert(f2.average_edge_size() - 3.0 < 0.0001);

    HyperGraphFamily f3({3, 21}, {89, 11});
    assert(f3.average_edge_size() - 4.98 < 0.0001);
}

int main() {
    test_hyper_graph();
    test_hyper_graph_family();
    test_threshold();
    std::cout << "OK" << std::endl;
}

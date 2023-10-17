#include <iostream>
#include <cassert>

#include "hyper_graph.h"
#include "hyper_graph_family.h"

void test_hyper_graph() {
    HyperGraph g1(5, {{0, 0, 4}, {4, 1, 1}});
    assert(g1.is_core_empty());

    HyperGraph g2(5, {{0, 0, 1}, {0, 1, 1}});
    assert(!g2.is_core_empty());
}

void test_hyper_graph_family() {
    const uint32_t edge_count = 100000;

    HyperGraphFamily f1({3}, {1});
    assert(f1.sample(edge_count, 0.81).is_core_empty());
    assert(!f1.sample(edge_count, 0.83).is_core_empty());

    HyperGraphFamily f2({4}, {1});
    assert(f2.sample(edge_count, 0.76).is_core_empty());
    assert(!f2.sample(edge_count, 0.78).is_core_empty());

    HyperGraphFamily f3({5}, {1});
    assert(f3.sample(edge_count, 0.69).is_core_empty());
    assert(!f3.sample(edge_count, 0.71).is_core_empty());

    HyperGraphFamily f4({3, 21}, {89, 11});
    assert(f4.sample(edge_count, 0.91).is_core_empty());
    assert(!f4.sample(edge_count, 0.93).is_core_empty());
}

void test_binary_search_for_alpha() {
    const uint32_t edge_count = 100000;
    float epsilon = 1e-6;

    HyperGraphFamily f1({3}, {1});
    assert(f1.binary_search(epsilon) > 0.81);
    assert(f1.binary_search(epsilon) < 0.83);


    HyperGraphFamily f2({4}, {1});
    assert(f2.binary_search(epsilon) > 0.76);
    assert(f2.binary_search(epsilon) < 0.78);


    HyperGraphFamily f3({5}, {1});
    assert(f3.binary_search(epsilon) > 0.69);
    assert(f3.binary_search(epsilon) < 0.71);

    HyperGraphFamily f4({3, 21}, {89, 11});
    assert(f4.binary_search(epsilon) > 0.91);
    assert(f4.binary_search(epsilon) < 0.93);
}

int main() {
    test_hyper_graph();
    test_hyper_graph_family();
    test_binary_search_for_alpha();
    std::cout << "OK" << std::endl;
}

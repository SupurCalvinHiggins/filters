#include <iostream>
#include <cassert>
#include <cmath>
#include <vector>

#include "hyper.h"

void test_hyper_graph() {
    HyperGraph g1(4, {{0, 0, 4}, {4, 1, 1}});
    assert(g1.is_core_empty());

    HyperGraph g2(4, {{0, 0, 1}, {0, 1, 1}});
    assert(!g2.is_core_empty());
}

void test_coupled_hyper_graph_family() {
    std::mt19937 rng(0);

    // CoupledHyperGraphFamily f1({3}, {1});
    // assert(f1.sample(0.81, rng).is_core_empty());
    // assert(!f1.sample(0.83, rng).is_core_empty());

    // CoupledHyperGraphFamily f2({4}, {1});
    // assert(f2.sample(0.76, rng).is_core_empty());
    // assert(!f2.sample(0.78, rng).is_core_empty());

    // CoupledHyperGraphFamily f3({5}, {1});
    // assert(f3.sample(0.69, rng).is_core_empty());
    // assert(!f3.sample(0.71, rng).is_core_empty());

    // CoupledHyperGraphFamily f4({3, 21}, {89, 11});
    // assert(f4.sample(0.91, rng).is_core_empty());
    // assert(!f4.sample(0.93, rng).is_core_empty());

    // CoupledHyperGraphFamily f5({3}, {1});
    // assert(f5.sample(0.91, rng).is_core_empty());
    // assert(!f5.sample(0.93, rng).is_core_empty());
}

void test_is_core_empty_threshold() {
    std::mt19937 rng(0);

    {
        CoupledHyperGraphFamily family({3}, {1});
        const auto threshold = is_core_empty_threshold(family, rng);
        std::cout << threshold << std::endl;
        // assert(threshold > 0.81);
        // assert(threshold < 0.83);
    }
    
    {
        CoupledHyperGraphFamily family({4}, {1});
        const auto threshold = is_core_empty_threshold(family, rng);
        std::cout << threshold << std::endl;
        // assert(threshold > 0.81);
        // assert(threshold < 0.83);
    }

    // CoupledHyperGraphFamily f2({4}, {1});
    // assert(f2.threshold(rng,0.0, 1.0, 0.0001) > 0.76);
    // assert(f2.threshold(rng,0.0,1.0, 0.0001) < 0.78);


    // CoupledHyperGraphFamily f3({5}, {1});
    // assert(f3.threshold(edge_count,rng,0.0,1.0, 0.0001) > 0.69);
    // assert(f3.threshold(edge_count,rng,0.0,1.0, 0.0001) < 0.71);

    // CoupledHyperGraphFamily f4({3, 21}, {89, 11});
    // assert(f4.threshold(edge_count,rng,0.0,1.0, 0.0001) > 0.91);
    // assert(f4.threshold(edge_count,rng,0.0,1.0, 0.0001) < 0.93);
}

void test_average_edge_size() {
    // CoupledHyperGraphFamily f1({3}, {1});
    // assert(f1.average_edge_size() - 3.0 < 0.0001);

    // CoupledHyperGraphFamily f2({3}, {4});
    // assert(f2.average_edge_size() - 3.0 < 0.0001);

    // CoupledHyperGraphFamily f3({3, 21}, {89, 11});
    // assert(f3.average_edge_size() - 4.98 < 0.0001);
}

int main() {
    test_hyper_graph();
    test_coupled_hyper_graph_family();
    test_is_core_empty_threshold();
    std::cout << "OK" << std::endl;
}

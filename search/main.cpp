#include <iostream>

#include "hyper_graph.h"
#include "random_hyper_graph_family.h"
#include "message_queue.h"

int main() {
    RandomHyperGraphFamily family({3}, {1});
    for (const auto load_factor : {0.1, 0.3, 0.5, 0.7, 0.8, 0.81, 0.83, 0.9}) {
        std::cout << "alpha=" << load_factor << " is_core_empty=" << family.sample(100000, load_factor).is_core_empty() << std::endl;
    }

    // HyperGraph graph(5, {{0, 0, 1}, {1, 2, 2}});
    // std::cout << graph.is_core_empty() << std::endl;
}
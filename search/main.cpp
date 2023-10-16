#include <iostream>
#include "hypergraph.h"


int main() {
    RandomHyperGraphFamily family({3}, {1});
    std::cout << family.sample(100000, 0.9).is_core_empty() << std::endl;
}
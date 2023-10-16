#include <iostream>
#include "hypergraph.h"


int main() {
    std::default_random_engine rng;
    HyperGraph g(3, 10000, 0.9, rng);
    std::cout << g.peelable() << std::endl;
}
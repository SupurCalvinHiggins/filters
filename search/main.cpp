#include <iostream>

#include "hyper_graph.h"
#include "hyper_graph_family.h"
#include "message_queue.h"

// need a function that yields all combinations of edges up to threshold
// need a function that yields all combinations of probs
// need to implement coupling
// need to seperate threshold from hypergraphfamily and make into template

int main() {
    // terrible code that doesnt really work at all :)
    for (int e1 = 3; e1 < 7; ++e1) {
        for (int e2 = e1 + 1; e2 < 7; ++e2) {
            for (int e3 = e2 + 1; e3 < 7; ++e3) {
                for (int p1 = 1; p1 <= 100; ++p1) {
                    for (int p2 = 1; p2 <= 100 - p1; ++p2) {
                        double ehash =  (double) (e1 * p1) / (double)(p1 + p2) + (double) (e2 * p2) / (double)(p1 + p2);
                        if (ehash > 4) continue;
                        HyperGraphFamily f({e1, e2, e3}, {p1, p2, p2});
                        if (!f.sample(1000, 0.85).is_core_empty()) continue;
                        std::cout << ehash << " " << f.threshold(100000, 0.0, 1.0) << " " << e1 << " " << e2 << " " << std::endl;
                    }
                }
            }
        }
    }
}

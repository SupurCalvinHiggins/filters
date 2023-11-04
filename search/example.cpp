#include <iostream>
#include <cassert>
#include <cmath>
#include <vector>

#include "hyper.h"


int main() {
	std::mt19937 rng(0);
	CoupledHyperGraphFamily family({3, 12}, {73, 9});
	std::cout << "average edge size: " << family.average_edge_size() << std::endl;
	const auto threshold = is_core_empty_threshold(family, rng);
	std::cout << "threshold: " << threshold << std::endl;
}

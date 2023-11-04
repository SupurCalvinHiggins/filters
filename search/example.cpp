#include <iostream>
#include <cassert>
#include <cmath>
#include <vector>

#include "hyper.h"


int main() {
	std::random_device rd;
	// Replace the call to rd with a literal 0 to ensure consistent results.
	std::mt19937 rng(rd());

	// TODO: Change me.
	// The first array is a list of edge sizes. Edge sizes should probably be >2.
	// The second array is a list of edge weights. The weights are relative. If the edge 
	// sizes were {3, 4, 5} and the weights were {1, 1, 2}, 25% of the edges would be 
	// size 3, 25% would be size 4 and 50% would be size 5.
	// The last number is the edge count. Higher values will take longer but produce
	// better results and are more accurate. Results can ONLY be compared with a 
	// consistent edge count. 1 million edges takes minutes for a single graph.
	CoupledHyperGraphFamily family({3, 12}, {73, 9}, 1000000);

	// Lower is better. It's the average number of hashes. We should focus on between 3 
	// and 4 hashes on average. It's not possible to win if we go outside that range.
	std::cout << "average edge size: " << family.average_edge_size() << std::endl;

	// Higher is better. Binary fuse filters get 0.889 with 3 hashes and 0.930 with 
	// 4 hashes. To get more accurate results, you can average this value over multiple
	// runs.
	const auto threshold = is_core_empty_threshold(family, rng);
	std::cout << "threshold: " << threshold << std::endl;
	
	// Lower is better. 0% is the best possible. Calcuated from the threshold.
	std::cout << "percent overhead: " << 100 * ((1 / threshold) - 1) << std::endl;
}

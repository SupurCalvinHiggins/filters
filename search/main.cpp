#include <iostream>
<<<<<<< HEAD
#include <random>
#include <cassert>

=======
#include <thread>
#include <fstream>
>>>>>>> a92d0b2 (single producer many consumers + csv output)
#include "hyper_graph.h"
#include "hyper_graph_family.h"
#include "message_queue.h"

// need a function that yields all combinations of edges up to threshold
// need a function that yields all combinations of probs
// need to implement coupling
// need to seperate threshold from hypergraphfamily and make into template

<<<<<<< HEAD
HyperGraphFamily random_hyper_graph_family(double max_average_edge_size, uint32_t max_edge_size, uint32_t max_edge_count, uint32_t max_weight, uint32_t seed) {
    assert(max_edge_size >= 3);
    assert(max_average_edge_size >= 3.0);
    assert(max_edge_count >= 1);

    std::mt19937 rng(seed);
    std::uniform_int_distribution<uint32_t> edge_size_dist(3, max_edge_size);
    std::uniform_int_distribution<uint32_t> edge_count_dist(1, max_edge_count);

    while (1) {
        auto edge_size = edge_size_dist(rng);

        std::vector<uint32_t> edge_sizes;
        for (uint32_t i = 0; i < edge_size; ++i)
            edge_sizes.push_back(edge_count_dist(rng));

        std::vector<uint32_t> edge_weights;
        for (uint32_t i = 0; i < edge_size; ++i)
            edge_weights.push_back(edge_size_dist(rng));

        HyperGraphFamily f(std::move(edge_sizes), std::move(edge_weights));

        if (f.average_edge_size() < max_average_edge_size)
            return f;
    }
}

int main() {
    // terrible code that doesnt really work at all :)
    auto f = random_hyper_graph_family(5.0, 30, 7, 10, 42);
=======
void producer(MessageQueue<HyperGraphFamily> &queue) {
    while (true) {
        if (queue.size() < 50) {
            HyperGraphFamily f1({3, 21}, {89, 11});
            queue.push(std::move(f1));
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Adjust the delay as needed
        }
    }
}

void consumer(MessageQueue<HyperGraphFamily> &queue, const std::string &file_number, int max) {
    std::ofstream outfile("data_" + file_number + ".txt");

    while (true) {
        auto familyOptional = queue.pop();

        if (familyOptional) {
            HyperGraphFamily family = std::move(familyOptional.value());

            for (int i = 0; i < family.edge_sizes_.size(); ++i) {
                outfile << family.edge_sizes_[i] << ",";
            }

            for (int i = 0; i < max - family.edge_sizes_.size(); ++i) {
                outfile << ",";
            }

            for (int i = 0; i < family.edge_weights_.size(); ++i) {
                outfile << family.edge_weights_[i] << ",";
            }

            for (int i = 0; i < max - family.edge_weights_.size(); ++i) {
                outfile << ",";
            }

            outfile << family.window_size_ << ",";

            auto family_threshold = family.threshold(100000);
            outfile << family_threshold << std::endl;
        }
    }
    outfile.close();
}



int main() {
    MessageQueue<HyperGraphFamily> queue;
    std::thread single_producer_thread = std::thread(producer, std::ref(queue));

    std::vector<std::thread> multiple_consumer_thread;

    uint32_t consumer_count = 5;
    int max = 3;
    for (int i = 0; i < consumer_count; i++) {
        multiple_consumer_thread.push_back(std::thread(consumer, std::ref(queue), std::string(std::to_string(i)), max));
    }

    single_producer_thread.join();

    for (auto &thread : multiple_consumer_thread) {
        thread.join();
    }
>>>>>>> a92d0b2 (single producer many consumers + csv output)
}

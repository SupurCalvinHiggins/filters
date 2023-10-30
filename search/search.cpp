#include <iostream>
#include <random>
#include <cassert>
#include <thread>
#include <fstream>
#include <functional>
#include <atomic>
#include "hyper_graph.h"
#include "hyper_graph_family.h"
#include "message_queue.h"

std::atomic<bool> global_flag(true);

// need a function that yields all combinations of edges up to threshold
// need a function that yields all combinations of probs
// need to implement coupling
// need to seperate threshold from hypergraphfamily and make into template

HyperGraphFamily random_hyper_graph_family(double max_average_edge_size, uint32_t max_edge_count, uint32_t max_edge_size, uint32_t max_weight, std::mt19937& rng) {
    assert(max_average_edge_size >= 3.0);
    assert(max_edge_count >= 2);
    assert(max_edge_size >= 3);

    std::uniform_int_distribution<uint32_t> edge_count_dist(2, max_edge_count);
    std::uniform_int_distribution<uint32_t> edge_size_dist(3, max_edge_size);
    std::uniform_int_distribution<uint32_t> edge_weight_dist(1, max_weight);

    while (1) {
        const auto edge_count = edge_count_dist(rng);

        std::vector<uint32_t> edge_sizes;
        for (uint32_t i = 0; i < edge_count; ++i)
            edge_sizes.push_back(edge_size_dist(rng));

        std::vector<uint32_t> edge_weights;
        for (uint32_t i = 0; i < edge_count; ++i)
            edge_weights.push_back(edge_weight_dist(rng));

        HyperGraphFamily f(std::move(edge_sizes), std::move(edge_weights));

        if (f.average_edge_size() < max_average_edge_size)
            return f;
    }
}

void producer(MessageQueue<HyperGraphFamily> &queue, std::function<HyperGraphFamily()> random_hyper_graph_family_func) {
    while (global_flag) {
        if (queue.size() > 50) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }
        std::cout << queue.size() << std::endl;
        queue.push(std::move(random_hyper_graph_family_func()));
    }
}

void consumer(uint32_t id, MessageQueue<HyperGraphFamily> &queue, uint32_t max_edge_count, std::mt19937 rng) {
    std::ofstream os(std::to_string(id) + ".csv");

    while (global_flag) {
        const auto optional_family = queue.pop();

        if (optional_family) {
            const HyperGraphFamily family = std::move(optional_family.value());

            for (uint32_t i = 0; i < max_edge_count; ++i) {
                if (i < family.edge_sizes_.size())
                    os << family.edge_sizes_[i];
                os << ",";
            }

            for (uint32_t i = 0; i < max_edge_count; ++i) {
                if (i < family.edge_weights_.size())
                    os << family.edge_weights_[i];
                os << ",";
            }

            os << family.window_size_ << ",";

            if (!family.sample(100000, 0.92, rng).is_core_empty()) {
                os << std::endl;
                continue;
            }

            const auto threshold = family.threshold(100000,0.0,1.0, 0.0001,rng);
            os << threshold << std::endl;
        }
    }
}

int main() {
    // TODO: Take these as CLAs.
    const uint32_t consumer_count = 5;
    const uint32_t producer_count = 1;

    // TODO: Take these as CLAs.
    const double max_average_edge_size = 4.5;
    const uint32_t max_edge_count = 5;
    const uint32_t max_edge_size = 30;
    const uint32_t max_weight = 10;
    const uint32_t seed = 0;

    // Create message queue.
    MessageQueue<HyperGraphFamily> queue;

    // Create consumers.
    std::vector<std::thread> consumers;
    for (uint32_t i = 0; i < consumer_count; ++i)
        consumers.emplace_back(consumer, i, std::ref(queue), max_edge_count);

    // Create producers.
    // WARN: rng is unsafe to share yet is shared.
    assert(producer_count == 1);
    std::mt19937 rng(seed);
    auto random_hyper_graph_family_func = [=, &rng](){
        return random_hyper_graph_family(max_average_edge_size, max_edge_count, max_edge_size, max_weight, rng);
    };

    // WARN: producers do not currently receive different seeds.
    assert(producer_count == 1);
    std::vector<std::thread> producers;
    for (uint32_t i = 0; i < producer_count; ++i)
        producers.emplace_back(producer, std::ref(queue), random_hyper_graph_family_func);

    // Join consumers.
    for (auto& thread : consumers)
        thread.join();

    // Join producers.
    for (auto& thread : producers)
        thread.join();
}

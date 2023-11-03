#include <iostream>
#include <random>
#include <cassert>
#include <thread>
#include <fstream>
#include <functional>
#include <atomic>
#include "hyper.h"
#include "message_queue.h"

std::atomic<bool> global_flag{ true };

void producer(MessageQueue<CoupledHyperGraphFamily> &queue, std::function<CoupledHyperGraphFamily()> random_hyper_graph_family_func) {
    while (global_flag) {
        if (queue.size() > 100) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }
        queue.push(std::move(random_hyper_graph_family_func()));
    }
}

void consumer(uint32_t id, MessageQueue<CoupledHyperGraphFamily> &queue, uint32_t max_edge_count) {
    std::ofstream os(std::to_string(id) + ".csv");
    std::mt19937 rng(0);
    
    while (global_flag) {
        const auto optional_family = queue.pop();

        if (optional_family) {
            const auto family = std::move(optional_family.value());

            for (uint32_t i = 0; i < max_edge_count; ++i) {
                if (i < family.edge_sizes.size())
                    os << family.edge_sizes[i];
                os << ",";
            }

            for (uint32_t i = 0; i < max_edge_count; ++i) {
                if (i < family.edge_weights.size())
                    os << family.edge_weights[i];
                os << ",";
            }

            // TODO: Change me.
            os << family.average_edge_size() << ",";

            if (!family.sample(0.87, rng).is_core_empty()) {
                os << std::endl;
                continue;
            }

            const auto threshold = is_core_empty_threshold(family, rng, 0.87, 1.0);
            os << threshold << std::endl;
        }
    }
}

int main() {
    // TODO: Take these as CLAs.
    const uint32_t consumer_count = 12;
    const uint32_t producer_count = 1;

    // TODO: Take these as CLAs.
    const double max_average_edge_size = 4;
    const uint32_t max_edge_count = 3;
    const uint32_t max_edge_size = 10;
    const uint32_t max_weight = 100;
    const uint32_t seed = 0;

    // Create message queue.
    MessageQueue<CoupledHyperGraphFamily> queue;

    // Create consumers.
    std::vector<std::thread> consumers;
    for (uint32_t i = 0; i < consumer_count; ++i)
        consumers.emplace_back(consumer, i, std::ref(queue), max_edge_count);

    // Create producers.
    // WARN: rng is unsafe to share yet is shared.
    assert(producer_count == 1);
    std::mt19937 rng(seed);
    auto random_hyper_graph_family_func = [=, &rng](){
        return random_family(max_average_edge_size, max_edge_count, max_edge_size, max_weight, rng);
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

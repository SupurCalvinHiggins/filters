#include <iostream>
#include <cassert>

#include "hyper_graph.h"
#include "hyper_graph_family.h"
#include "message_queue.h"
#include <thread>

struct threadInfo {
    bool finished = false;
};

void test_hyper_graph() {
    HyperGraph g1(5, {{0, 0, 4}, {4, 1, 1}});
    assert(g1.is_core_empty());

    HyperGraph g2(5, {{0, 0, 1}, {0, 1, 1}});
    assert(!g2.is_core_empty());
}

void test_hyper_graph_family() {
    const uint32_t edge_count = 100000;

    HyperGraphFamily f1({3}, {1});
    assert(f1.sample(edge_count, 0.81).is_core_empty());
    assert(!f1.sample(edge_count, 0.83).is_core_empty());

    HyperGraphFamily f2({4}, {1});
    assert(f2.sample(edge_count, 0.76).is_core_empty());
    assert(!f2.sample(edge_count, 0.78).is_core_empty());

    HyperGraphFamily f3({5}, {1});
    assert(f3.sample(edge_count, 0.69).is_core_empty());
    assert(!f3.sample(edge_count, 0.71).is_core_empty());

    HyperGraphFamily f4({3, 21}, {89, 11});
    assert(f4.sample(edge_count, 0.91).is_core_empty());
    assert(!f4.sample(edge_count, 0.93).is_core_empty());
}

void test_threshold_for_alpha() {
    const uint32_t edge_count = 100000;

    HyperGraphFamily f1({3}, {1});
    assert(f1.threshold(edge_count) > 0.81);
    assert(f1.threshold(edge_count) < 0.83);


    HyperGraphFamily f2({4}, {1});
    assert(f2.threshold(edge_count) > 0.76);
    assert(f2.threshold(edge_count) < 0.78);


    HyperGraphFamily f3({5}, {1});
    assert(f3.threshold(edge_count) > 0.69);
    assert(f3.threshold(edge_count) < 0.71);

    HyperGraphFamily f4({3, 21}, {89, 11});
    assert(f4.threshold(edge_count) > 0.91);
    assert(f4.threshold(edge_count) < 0.93);
}

void test_average_edge_size() {
    HyperGraphFamily f1({3}, {1});
    assert(f1.average_edge_size() - 3.0 < 0.0001);

    HyperGraphFamily f2({3}, {4});
    assert(f2.average_edge_size() - 3.0 < 0.0001);

    HyperGraphFamily f3({3, 21}, {89, 11});
    assert(f3.average_edge_size() - 4.98 < 0.0001);
}

// void producer(MessageQueue<HyperGraphFamily> &queue, threadInfo& thread) {
//     while (!thread.finished) {
//         HyperGraphFamily f1({ static_cast<unsigned int>(rand() % 20)}, { static_cast<unsigned int>(rand() % 20)});
//         queue.push(std::move(f1));
//         std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Adjust the delay as needed

//     }
// }

// void consumer(MessageQueue<HyperGraphFamily> &queue, threadInfo& thread) {
//     while (!thread.finished) {

//         auto familyOptional = queue.pop();

//         HyperGraphFamily family = std::move(familyOptional.value());
//         std::cout << "alpha " << family.threshold(1e-6) << std::endl;

//     }
// }

// void test_message_queue() {
//     MessageQueue<HyperGraphFamily> queue;

//     std::vector<threadInfo> producer_threads;
//     std::vector<threadInfo> consumer_threads;

//     for (int i = 0; i < 8; i++) {
//         threadInfo thread;
//         if (i % 2 == 0)
//             producer_threads.push_back(thread);
//         else
//             consumer_threads.push_back(thread);
//     }

//     std::vector<std::thread> running_producers;
//     std::vector<std::thread> running_consumers;

//     for (int i = 0; i < producer_threads.size(); i++) {
//         running_producers.push_back(std::thread(producer, std::ref(queue), std::ref(producer_threads[i])));
//     }

//     for (int i = 0; i < consumer_threads.size(); i++) {
//         running_consumers.push_back(std::thread(consumer, std::ref(queue), std::ref(consumer_threads[i])));
//     }

//     for (auto &thread : running_producers) {
//         if (thread.joinable()) {
//             thread.join();
//         }
//     }

//     for (auto &thread : running_consumers) {
//         if (thread.joinable()) {
//             thread.join();
//         }
//     }
// }




int main() {
    test_hyper_graph();
    test_hyper_graph_family();
    test_threshold_for_alpha();
    // test_message_queue();
    std::cout << "OK" << std::endl;
}

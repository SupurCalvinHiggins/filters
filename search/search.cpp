//#include <iostream>
//#include <random>
//#include <cassert>
//#include <thread>
//#include <fstream>
//#include <functional>
//#include <string>
//#include <chrono>
//#include <ctime>
//#include <sys/time.h>
//#include <fstream>
//#include <atomic>
//#include "hyper.h"
//#include "message_queue.h"
//
//std::atomic<bool> global_flag{ true };
//std::vector<std::vector<double>> edge_sizes;
//std::vector<std::vector<double>> edge_weights;
//std::vector<double> average_edge_sizes;
//std::vector<double> thresholds;
//
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <vector>
//
//void parse_csv() {
//    // Open the CSV file
//    std::ifstream file("bad.csv");
//
//    std::string line;
//
//    std::vector<double> temp;
//    while (std::getline(file, line)) {
//        std::istringstream ss(line);
//        std::string token;
//        // Parse the first 5 elements
//        for (int i = 0; i < 5; ++i) {
//            std::getline(ss, token, ',');
//            if (token.empty()) {
//                // If the token is empty, count it as 0
//            } else {
//                temp.push_back(std::stod(token));
//            }
//        }
//        edge_sizes.push_back(temp);
//        temp.clear();
//        // Parse the next 5 elements
//        for (int i = 0; i < 5; ++i) {
//            std::getline(ss, token, ',');
//            if (token.empty()) {
//                // If the token is empty, count it as 0
//                std::cout << "0 ";
//            } else {
//                temp.push_back(std::stod(token));
//            }
//        }
//        edge_weights.push_back(temp);
//        temp.clear();
//        // Parse the last 2 elements
////        for (int i = 0; i < 2; ++i) {
//        std::getline(ss, token, ',');
//        if (token.empty()) {
//            // If the token is empty, count it as 0
//            std::cout << "0 ";
//        } else {
//            average_edge_sizes.push_back(std::stod(token));
//        }
//
//        std::getline(ss, token, ',');
//        if (token.empty()) {
//            // If the token is empty, count it as 0
//            std::cout << "0 ";
//        } else {
//            thresholds.push_back(std::stod(token));
//        }
//        //}
//    }
//
//    file.close();
//}
//
//
//void producer(MessageQueue<CoupledHyperGraphFamily> &queue, std::function<CoupledHyperGraphFamily()> random_hyper_graph_family_func) {
//    while (global_flag) {
//        if (queue.size() > 100) {
//            // TODO: Remove. Not thread safe.
//            std::this_thread::sleep_for(std::chrono::milliseconds(100));
//            continue;
//        }
//        queue.push(std::move(random_hyper_graph_family_func()));
//    }
//}
//
//void consumer(uint32_t id, MessageQueue<CoupledHyperGraphFamily> &queue, uint32_t max_edge_count, std::string out_prefix) {
//    std::ofstream os(std::to_string(id) + "_" + out_prefix + ".csv");
//    std::mt19937 rng(0);
//
//    while (global_flag) {
//        const auto optional_family = queue.pop();
//
//        if (optional_family) {
//            const auto family = std::move(optional_family.value());
//
//            for (uint32_t i = 0; i < max_edge_count; ++i) {
//                if (i < family.edge_sizes.size())
//                    os << family.edge_sizes[i];
//                os << ",";
//            }
//
//            for (uint32_t i = 0; i < max_edge_count; ++i) {
//                if (i < family.edge_weights.size())
//                    os << family.edge_weights[i];
//                os << ",";
//            }
//
//            // TODO: Change me.
//            os << family.average_edge_size() << ",";
//
//            // if (!family.sample(0.89, rng).is_core_empty()) {
//            //     os << std::endl;
//            //     continue;
//            // }
//
//            const auto threshold = is_core_empty_threshold(family, rng, 0.89, 1.0);
//            os << threshold << std::endl;
//        }
//    }
//}
//
//void generate_cla_file(std::string file_name, char* argv[])
//{
//    std::ofstream out_file(file_name + ".txt");
//    out_file <<  "Consumer Count = " << argv[1] << std::endl
//    << "Producer Count = " << argv[2] << std::endl
//    <<  "Max Average Edge Size = " << argv[3] << std::endl
//    << "Max Edge Count = " << argv[4] << std::endl
//    << "Max Edge Size = " << argv[5] << std::endl
//    << "Max Weight = " << argv[6] << std::endl
//    << "Seed = " << argv[7] << std::endl;
//}
//
//std::string get_current_time()
//{
//    // Get the current time
//    auto now = std::chrono::system_clock::now();
//    auto now_time = std::chrono::system_clock::to_time_t(now);
//    auto now_tm = *std::localtime(&now_time);
//
//    // Get milliseconds using gettimeofday from sys/time.h
//    struct timeval tv;
//    gettimeofday(&tv, nullptr);
//
//    // Format the time components
//    char time_str[32];
//    std::strftime(time_str, sizeof(time_str), "%H:%M:%S", &now_tm);
//
//    // Add milliseconds
//    char ms_str[8];
//    std::snprintf(ms_str, sizeof(ms_str), ".%03d", static_cast<int>(tv.tv_usec / 1000));
//
//    // Combine the time and milliseconds
//    return std::string(time_str) + ms_str;
//}
//
//int main(int argc, char* argv[]) {
//
//    if (argc < 8)
//    {
//        std::cerr << "Error in args: ./prog" <<
//         "#consumer #producers #average_edge_size #max_edge_count #max_edge_size #max_weight #seed" << std::endl;
//
//        exit(EXIT_FAILURE);
//    }
//
//    const uint32_t consumer_count = std::stoi(argv[1]);
//    const uint32_t producer_count = std::stoi(argv[2]);
//
//    const double max_average_edge_size = std::stoi(argv[3]);
//    const uint32_t max_edge_count = std::stoi(argv[4]);
//    const uint32_t max_edge_size = std::stoi(argv[5]);
//    const uint32_t max_weight = std::stoi(argv[6]);
//    const uint32_t seed = std::stoi(argv[7]);
//    const std::string out_prefix = get_current_time();
//    generate_cla_file(out_prefix, argv);
//
//    // Create message queue.
//    MessageQueue<CoupledHyperGraphFamily> queue;
//
//    // Create consumers.
//    std::vector<std::thread> consumers;
//    for (uint32_t i = 0; i < consumer_count; ++i)
//        consumers.emplace_back(consumer, i, std::ref(queue), max_edge_count, out_prefix);
//
//    // Create producers.
//    // WARN: rng is unsafe to share yet is shared.
//    assert(producer_count == 1);
//    std::mt19937 rng(seed);
//    auto random_hyper_graph_family_func = [=, &rng](){
//        return random_family(max_average_edge_size, max_edge_count, max_edge_size, max_weight, rng);
//    };
//
//    // WARN: producers do not currently receive different seeds.
//    assert(producer_count == 1);
//    std::vector<std::thread> producers;
//    for (uint32_t i = 0; i < producer_count; ++i)
//        producers.emplace_back(producer, std::ref(queue), random_hyper_graph_family_func);
//
//    // Join consumers.
//    for (auto& thread : consumers)
//        thread.join();
//
//    // Join producers.
//    for (auto& thread : producers)
//        thread.join();
//}

#include <iostream>
#include <random>
#include <cassert>
#include <thread>
#include <fstream>
#include <functional>
#include <string>
#include <chrono>
#include <ctime>
#include <sys/time.h>
#include <fstream>
#include <atomic>
#include "hyper.h"
#include "message_queue.h"

std::atomic<bool> global_flag{ true };
std::vector<std::vector<uint32_t>> edge_sizes;
std::vector<std::vector<uint32_t>> edge_weights;
std::vector<double> average_edge_sizes;
std::vector<double> thresholds;

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

void print_vector(std::vector<uint32_t> vec) {
    for (auto &i : vec) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

void parse_csv() {
    // Open the CSV file
    std::ifstream file("bad.csv");

    std::string line;

    std::vector<uint32_t> temp;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string token;
        // Parse the first 5 elements
        for (int i = 0; i < 5; ++i) {
            std::getline(ss, token, ',');
            if (token.empty()) {
                // If the token is empty, count it as 0
                temp.push_back(0);
            } else {
                // Convert to uint32_t
//                std::cout << static_cast<uint32_t>(std::stod(token));
                temp.push_back(static_cast<uint32_t>(std::stod(token)));
            }
        }
        edge_sizes.push_back(temp);
        temp.clear();
        // Parse the next 5 elements
        for (int i = 0; i < 5; ++i) {
            std::getline(ss, token, ',');
            if (token.empty()) {
                // If the token is empty, count it as 0
                temp.push_back(0);
            } else {
                // Convert to uint32_t
                temp.push_back(static_cast<uint32_t>(std::stod(token)));
            }
        }
        edge_weights.push_back(temp);
        temp.clear();
        // Parse the last 2 elements
        std::getline(ss, token, ',');
        if (token.empty()) {
            // If the token is empty, count it as 0
            average_edge_sizes.push_back(0);
        } else {
            // Convert to uint32_t
            average_edge_sizes.push_back(static_cast<uint32_t>(std::stod(token)));
        }

        std::getline(ss, token, ',');
        if (token.empty()) {
            // If the token is empty, count it as 0
            thresholds.push_back(0);
        } else {
            // Convert to uint32_t
            thresholds.push_back(static_cast<uint32_t>(std::stod(token)));
        }
    }

    file.close();
}


//CoupledHyperGraphFamily(
//        std::vector<uint32_t>&& edge_sizes,
//std::vector<uint32_t>&& edge_weights,
//        uint32_t edge_count = 1000000
//) : edge_sizes(edge_sizes), edge_weights(edge_weights), edge_count(edge_count) {
//    assert(edge_sizes.size() > 0);
//    assert(edge_weights.size() > 0);
//    assert(edge_sizes.size() == edge_weights.size());
//}
//myVector.erase(std::remove(myVector.begin(), myVector.end(), 0), myVector.end());

// Modify the definition of random_hyper_graph_family_func to accept edge_sizes and edge_weights
// Modify the producer function to pass edge_sizes and edge_weights to random_hyper_graph_family_func
void producer(MessageQueue<CoupledHyperGraphFamily> &queue) {
//    while (global_flag) {
        for (int i = 0; i < edge_weights.size(); i++) {
            if (queue.size() > 100) {
                // TODO: Remove. Not thread safe.
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            } else {
//                edge_sizes[i].erase(std::remove(edge_sizes[i].begin(), edge_sizes[i].end(), 0), edge_sizes[i].end());
//                edge_weights[i].erase(std::remove(edge_weights[i].begin(), edge_weights[i].end(), 0), edge_weights[i].end());
                queue.push(CoupledHyperGraphFamily(std::move(edge_sizes[i]), std::move(edge_weights[i])));
            }
        }
    //}
}


void consumer(uint32_t id, MessageQueue<CoupledHyperGraphFamily> &queue, uint32_t max_edge_count, std::string out_prefix) {
    std::ofstream os(std::to_string(id) + "_" + out_prefix + ".csv");
    std::mt19937 rng(0);

    while (global_flag) {
        const auto optional_family = queue.pop();
        if (optional_family) {
            std::cout << "HERE";
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

            // if (!family.sample(0.89, rng).is_core_empty()) {
            //     os << std::endl;
            //     continue;
            // }

            const auto threshold = is_core_empty_threshold(family, rng, 0, 1.0);
            os << threshold << std::endl;
        }
    }
}

void generate_cla_file(std::string file_name, char* argv[])
{
    std::ofstream out_file(file_name + ".txt");
    out_file <<  "Consumer Count = " << argv[1] << std::endl
    << "Producer Count = " << argv[2] << std::endl
    <<  "Max Average Edge Size = " << argv[3] << std::endl
    << "Max Edge Count = " << argv[4] << std::endl
    << "Max Edge Size = " << argv[5] << std::endl
    << "Max Weight = " << argv[6] << std::endl
    << "Seed = " << argv[7] << std::endl;
}

std::string get_current_time()
{
    // Get the current time
    auto now = std::chrono::system_clock::now();
    auto now_time = std::chrono::system_clock::to_time_t(now);
    auto now_tm = *std::localtime(&now_time);

    // Get milliseconds using gettimeofday from sys/time.h
    struct timeval tv;
    gettimeofday(&tv, nullptr);

    // Format the time components
    char time_str[32];
    std::strftime(time_str, sizeof(time_str), "%H:%M:%S", &now_tm);

    // Add milliseconds
    char ms_str[8];
    std::snprintf(ms_str, sizeof(ms_str), ".%03d", static_cast<int>(tv.tv_usec / 1000));

    // Combine the time and milliseconds
    return std::string(time_str) + ms_str;
}

int main(int argc, char* argv[]) {

    if (argc < 8)
    {
        std::cerr << "Error in args: ./prog" <<
         "#consumer #producers #average_edge_size #max_edge_count #max_edge_size #max_weight #seed" << std::endl;

        exit(EXIT_FAILURE);
    }

    const uint32_t consumer_count = std::stoi(argv[1]);
    const uint32_t producer_count = std::stoi(argv[2]);

//    const double max_average_edge_size = std::stoi(argv[3]);
    const uint32_t max_edge_count = std::stoi(argv[4]);
//    const uint32_t max_edge_size = std::stoi(argv[5]);
//    const uint32_t max_weight = std::stoi(argv[6]);
    const uint32_t seed = std::stoi(argv[7]);
    const std::string out_prefix = get_current_time();
    generate_cla_file(out_prefix, argv);
    parse_csv();
    // Create message queue.
    MessageQueue<CoupledHyperGraphFamily> queue;

    // Create consumers.
    std::vector<std::thread> consumers;
    for (uint32_t i = 0; i < consumer_count; ++i)
        consumers.emplace_back(consumer, i, std::ref(queue), max_edge_count, out_prefix);

    // Create producers.
    // WARN: rng is unsafe to share yet is shared.
    assert(producer_count == 1);
    std::mt19937 rng(seed);
////    auto random_hyper_graph_family_func = [=, &rng](){
////        return random_family(max_average_edge_size, max_edge_count, max_edge_size, max_weight, rng);
////    };
//
//    // WARN: producers do not currently receive different seeds.
    assert(producer_count == 1);
    std::vector<std::thread> producers;
    for (uint32_t i = 0; i < producer_count; ++i)
        producers.emplace_back(producer, std::ref(queue));

    // Join consumers.
    for (auto& thread : consumers)
        thread.join();

    // Join producers.
    for (auto& thread : producers)
        thread.join();
}


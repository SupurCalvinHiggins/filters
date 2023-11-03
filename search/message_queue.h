#pragma once

#include <queue>
#include <mutex>
#include <optional>

template <typename T>
class MessageQueue {
private:
    std::queue<T> q_;
    std::mutex mutex_;

public:
    void push(T&& msg);
    std::optional<T> pop();
    int size() {
        return q_.size();
    }
};

#include "message_queue.tpp"

#pragma once

#include <queue>
#include <mutex>
#include <optional>

template <typename T>
class MessageQueue {
private:
    std::queue<T> m_q;
    std::mutex m_mutex;

public:
    void push(T&& msg);
    std::optional<T> pop();
    int size() {
        return m_q.size();
    }
};

#include "message_queue.tpp"

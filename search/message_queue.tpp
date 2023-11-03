#pragma once

template <typename T>
void MessageQueue<T>::push(T&& msg) {
    std::lock_guard lock(m_mutex);
    m_q.push(msg);
}

template <typename T>
std::optional<T> MessageQueue<T>::pop() {
    std::lock_guard lock(m_mutex);
    if (m_q.empty())
        return std::nullopt;
    auto msg = std::move(m_q.front());
    m_q.pop();
    return std::make_optional(std::move(msg));
}
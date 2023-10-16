#pragma once

template <typename T>
void MessageQueue<T>::push(T&& msg) {
    std::lock_guard lock(mutex_);
    q_.push(msg);
}

template <typename T>
std::optional<T> MessageQueue<T>::pop() {
    std::lock_guard lock(mutex_);
    if (q_.empty())
        return std::nullopt;
    auto msg = std::move(q_.front());
    q_.pop();
    return std::make_optional(std::move(msg));
}
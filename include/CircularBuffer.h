#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>

template <typename T>
class CircularBuffer {
public:
    explicit CircularBuffer(size_t size);
    void push(const T& value);
    T pop();
    bool isEmpty() const;
    bool isFull() const;
    size_t size() const;
    T findMostPopularValue() const;

private:
    std::vector<T> buffer;
    size_t head;
    size_t tail;
    bool full;
};

template <typename T>
CircularBuffer<T>::CircularBuffer(size_t size) : buffer(size), head(0), tail(0), full(false) {}

template <typename T>
void CircularBuffer<T>::push(const T& value) {
    buffer[head] = value;
    if (full)
        tail = (tail + 1) % buffer.size();
    head = (head + 1) % buffer.size();
    full = (head == tail);
}

template <typename T>
T CircularBuffer<T>::pop() {
    if (isEmpty())
        throw std::runtime_error("Buffer is empty");

    T value = buffer[tail];
    full = false;
    tail = (tail + 1) % buffer.size();
    return value;
}

template <typename T>
bool CircularBuffer<T>::isEmpty() const {
    return (!full && (head == tail));
}

template <typename T>
bool CircularBuffer<T>::isFull() const {
    return full;
}

template <typename T>
size_t CircularBuffer<T>::size() const {
    return buffer.size();
}

template <typename T>
T CircularBuffer<T>::findMostPopularValue() const {
    std::unordered_map<T, size_t> counts;
    size_t maxCount = 0;
    T mostPopularValue;

    for (size_t i = 0; i < buffer.size(); ++i) {
        if (buffer[i] != false) {
            ++counts[buffer[i]];
            if (counts[buffer[i]] > maxCount) {
                maxCount = counts[buffer[i]];
                mostPopularValue = buffer[i];
            }
        }
    }

    if (maxCount == 0 && !counts[false] && isEmpty()) {
        return 0;
    }

    return mostPopularValue;
}

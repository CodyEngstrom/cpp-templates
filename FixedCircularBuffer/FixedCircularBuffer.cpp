#include "./FixedCircularBuffer.h"
#include <iostream>

// Adds item to buffer
// Returns 0 on success and 1 if buffer is full
template<typename T, size_t N>
int FixedCircularBuffer<T,N>::push(const T& item) {
    if (count < N) {
        buffer[back] = item;
        back = (back + 1) % N;
        count++;
    } else {
        std::cerr << "Buffer is full. Data not written." << std::endl;
        return 1;
    }
    return 0;
}

// Removes item from front of cirucular buffer
// Returns item value
template<typename T, size_t N>
T FixedCircularBuffer<T,N>::pop() {
    if (count == 0) {
        throw std::__throw_runtime_error("CircularBuffer empty");
    }
    T item = buffer[front];
    front = (front + 1) % N;
    count--;
    return item;
}

template<typename T, size_t N>
void FixedCircularBuffer<T, N>::printBuffer() {
    for (auto& item : buffer) {
        std::cout << item << std::endl;
    }
}

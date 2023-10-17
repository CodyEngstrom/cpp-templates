#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <array>
#include <cstddef>
#include <iostream>
#include <optional>


template<typename T, size_t N> 
class FixedCircularBuffer
{
private:
    std::array<T, N> buffer;
    size_t front;
    size_t back;
    size_t count;

public:
    FixedCircularBuffer(const T& defaultValue = T{}) : front(0), back(0), count(0){
        buffer.fill(defaultValue);
    };
    // ~FixedCircularBuffer();

    // Adds item to buffer
    // Returns 0 on success and 1 if buffer is full
    // int push(const T& item); 
    int push(const T& item) {
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
    // T pop(); 
    std::optional<T> pop() {
        if (count == 0) {
            return std::nullopt;
        }
        T item = buffer[front];
        front = (front + 1) % N;
        count--;
        return item;
    }

    // void printBuffer();
    void printBuffer() {
        for (auto& item : buffer) {
            std::cout << item << ", ";
        }
        std::cout << std::endl;
    }
};

#endif
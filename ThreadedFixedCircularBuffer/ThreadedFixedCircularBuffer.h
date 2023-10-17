#ifndef THREADEDFIXEDCIRCULARBUFFER_H
#define THREADEDFIXEDCIRCULARBUFFER_H

#include <array>
#include <cstddef>
#include <optional>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <queue>
#include <vector>

template <typename T, size_t N>
class ThreadedFixedCircularBuffer
{
private:
    std::array<T, N> buffer;
    size_t front;
    size_t back;
    size_t count;

    std::vector<size_t> producerIds;
    size_t indexCurProducer;
    size_t indexProducerInsert;
    
    
    std::mutex mtx; 
    std::condition_variable not_empty, not_full, prodSchedule;
public:
    ThreadedFixedCircularBuffer(const T& defaultValue = T{}) : front(0), back(0), count(0), indexCurProducer(0) {
        buffer.fill(defaultValue);
    };

    int push(const T& item, size_t producerId) {

        {
            std::unique_lock<std::mutex> scheduleLock(mtx);
            prodSchedule.wait(scheduleLock, [this, producerId] { return producerId == producerIds[indexCurProducer]; });
        }

        {
            std::unique_lock<std::mutex> lock(mtx);
            not_full.wait(lock, [this] { return count < N; });
            buffer[back] = item;
            back = (back + 1) % N;
            count++;
            indexCurProducer = (indexCurProducer + 1) % producerIds.size();
        }

        prodSchedule.notify_all();
        not_empty.notify_one();
        return 0;
    }

    T pop() {

        T item;
        {
            std::unique_lock<std::mutex> lock(mtx);
            not_empty.wait(lock, [this] { return count > 0; });
            item = buffer[front];
            front = (front + 1) % N;
            count--;
        }
        not_full.notify_all();
        
        return item;
    }

    void scheduleProducer(size_t producerId) {
        std::unique_lock<std::mutex> lock(mtx);
        producerIds.push_back(producerId);
        indexProducerInsert++;
    }

    void print() {
        for (int i = 0; i < N; i++) {
            std::cout << buffer[i] << ", ";
        }
        std::cout << std::endl;
    }

};


#endif
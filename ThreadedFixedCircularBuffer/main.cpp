#include "./ThreadedFixedCircularBuffer.h"

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include <chrono>
#include <iostream>
#include <fstream>

const size_t bufferSize = 2000;
const size_t nProducers = 1;
const size_t nConsumers = 1;
const size_t nItems = 100000;
ThreadedFixedCircularBuffer<int, bufferSize> buffer;





void producer(int id) {
    
    for(int i = 0; i < nItems; i++) {
        buffer.push(i, id);
    }
}

void consumer(int id) {
    std::string filename = "consumer" + std::to_string(id) + ".txt";
    std::ofstream file(filename);
    for(int i = 0; i < nItems*nProducers/nConsumers; i++) {
        int item = buffer.pop();
        file << item << std::endl;
    }
}



int main(int argc, char *argv[]) {

    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    


    for (int i = 0; i < nProducers; i++) {
        buffer.scheduleProducer(i);
    }
    

    std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();


    for (int i = 0; i < nProducers; i++) {
        producers.push_back(std::thread(producer, i));
    }
    for (int i = 0; i < nConsumers; i++) {
        consumers.push_back(std::thread(consumer, i));
    }


    for (auto& t : producers) {
        t.join();
    }

    for (auto& t : consumers) {
        t.join();
    }

    std::chrono::high_resolution_clock::time_point end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    std::cout << "Main function took " << duration.count() << " microseconds" << std::endl;

    // buffer.print();

    return 0;
}
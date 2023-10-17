#include "FixedCircularBuffer.h"

int main(int argc, char* argv[]) {
    FixedCircularBuffer<std::string,5> buffer;
    buffer.push("hello");
    buffer.push("world");
   
    buffer.printBuffer();
    return 0;
}
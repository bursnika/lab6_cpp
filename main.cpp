#include <iostream>
#include <queue>
#include "RandomGen.h"

int main() {
    std::queue<int> myQueue;
    int count = 15;

    std::cout << "--- Coroutine Demo ---" << std::endl;

    RandomGenerator gen = createRandomSequence(myQueue, count);

    int step = 1;
    while (!gen.done()) {
        std::cout << "[Main] Step " << step++ << ": Resuming..." << std::endl;

        gen.resume();

        if (!myQueue.empty()) {
            std::cout << "[Main] Queue back: " << myQueue.back() << std::endl;
        }

        if (gen.done()) {
            std::cout << "[Main] Coroutine finished." << std::endl;
        } else {
            std::cout << "[Main] Coroutine paused (Odd number)." << std::endl;
        }
        std::cout << "-----------------------" << std::endl;
    }

    std::cout << "\nTotal items in queue: " << myQueue.size() << std::endl;

    return 0;
}
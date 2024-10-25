#include "Dinner.hpp"

DinnerManager::DinnerManager(PhilosopherType type)  : mutexes(5, {}) {
    for (int i = 0; i < 5; ++i) {
        philosophers.push_back(PhilosopherFactory::createPhilosopher(type, mutexes[i], mutexes[(i + 1) % 5], i));
    }
}

void DinnerManager::dine() {
    std::vector<std::thread> threads;
    for (auto& philosopher : philosophers) {
        threads.emplace_back([&philosopher]() {
            philosopher->run(); 
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }
}

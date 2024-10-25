#include "PhilosopherImpl.hpp"

void NaivePhilosopher::eat()  {
    auto start_wait = std::chrono::steady_clock::now();

    std::unique_lock<std::shared_mutex> lock1(left_fork);
    std::unique_lock<std::shared_mutex> lock2(right_fork);

    record_waiting_time(start_wait);

    simulate_eating();
}

void StarvingPhilosopher::eat() {
    auto start_wait = std::chrono::steady_clock::now();

    bool acquired = false;
    while(!acquired) {
        if (left_fork.try_lock()) {
            if (right_fork.try_lock())
                acquired = true;
            else 
                left_fork.unlock();
        }
        
        std::this_thread::yield();
    }

    record_waiting_time(start_wait);
    
    simulate_eating();
    
    left_fork.unlock();
    right_fork.unlock();
}

void AsymmetricPhilosopher::eat() {
    auto start_wait = std::chrono::steady_clock::now();

    if (id % 2) {
        std::unique_lock<std::shared_mutex> lock1(left_fork);
        std::unique_lock<std::shared_mutex> lock2(right_fork); 
    } else {
        std::unique_lock<std::shared_mutex> lock2(right_fork);
        std::unique_lock<std::shared_mutex> lock1(left_fork);
    }

    record_waiting_time(start_wait);

    simulate_eating();
}

void ArbiterPhilosopher::eat() {    
    auto start_wait = std::chrono::steady_clock::now();

    arbiter.acquire();
    std::unique_lock<std::shared_mutex> lock1(left_fork);
    std::unique_lock<std::shared_mutex> lock2(right_fork);

    record_waiting_time(start_wait);

    simulate_eating();
    
    arbiter.release();
}

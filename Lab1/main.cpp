#include <iostream>
#include <thread>
#include <atomic>
#include <format>
#include <vector>

class Counter {
public:    
    static void run_unsafe_threads() {
        int operation_count {100000000};
        int counter {0};
        
        auto incrementator = [&]() {
            for(int i = 0; i < operation_count; ++i) 
                counter++;
        };

        auto decrementator = [&]() {
            for(int i = 0; i < operation_count; ++i)
                counter--;
        };

        auto start = std::chrono::high_resolution_clock::now();
        std::cout << std::format("Initial value of unsafe counter: {}", counter) << std::endl;
        std::jthread incrementing_thread(incrementator);
        std::jthread decrementing_thread(decrementator);
        
        incrementing_thread.join();
        decrementing_thread.join();
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << std::format("Final value of unsafe counter: {} (Duration: {} ms)", counter, duration) << std::endl;
    }

    static void run_safe_threads() {
        int operation_count {100000000};
        std::atomic<int> counter {0};
        auto incrementator = [&]() {
            for(int i = 0; i < operation_count; ++i)
                counter++;

        };

        auto decrementator = [&]() {
            for(int i = 0; i < operation_count; ++i)
                counter--;

        };
        
        auto start = std::chrono::high_resolution_clock::now();
        std::cout << std::format("Initial value of safe counter: {}", counter.load()) << std::endl;
        std::jthread incrementing_thread(incrementator);
        std::jthread decrementing_thread(decrementator);

        incrementing_thread.join();
        decrementing_thread.join();
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << std::format("Final value of safe counter: {} (Duration: {} ms)", counter.load(), duration) << std::endl;
    }

    static void creating_amaf_threads() {
        std::atomic<int> counter = 0;
        std::vector<std::thread> threads;

        for (;;) {
            threads.emplace_back([&counter]() {
                counter++;
                std::cout << counter.load() << std::endl;
                
                while (true) {
                    continue;
                }
            });
        }

        for (auto& thread : threads) {
            thread.join();
        }
    }
    
    void run() {
        run_unsafe_threads();
        run_safe_threads();
        // creating_amaf_threads();
    }
};

int main() {
    Counter counter;
    counter.run();
}
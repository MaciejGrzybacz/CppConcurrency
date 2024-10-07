#include <iostream>
#include <thread>
#include <atomic>
#include <format>

class Counter {
public:    
    static void run_unsafe_threads() {
        int operation_count {100000};
        int counter {0};
        
        auto incrementator = [&]() {
            for(int i = 0; i < operation_count; ++i) 
                counter++;
        };

        auto decrementator = [&]() {
            for(int i = 0; i < operation_count; ++i)
                counter--;
        };
        
        std::cout << std::format("Initial value of unsafe counter: {}", counter) << std::endl;
        std::jthread incrementing_thread(incrementator);
        std::jthread decrementing_thread(decrementator);
        
        incrementing_thread.join();
        decrementing_thread.join();

        std::cout << std::format("Final value of unsafe counter: {}", counter) << std::endl;
    }

    static void run_safe_threads() {
        int operation_count {100000};
        std::atomic<int> counter {0};
        auto incrementator = [&]() {
            for(int i = 0; i < operation_count; ++i)
                counter++;

        };

        auto decrementator = [&]() {
            for(int i = 0; i < operation_count; ++i)
                counter--;

        };
        
        std::cout << std::format("Initial value of safe counter: {}", counter.load()) << std::endl;
        std::jthread incrementing_thread(incrementator);
        std::jthread decrementing_thread(decrementator);

        incrementing_thread.join();
        decrementing_thread.join();
        
        std::cout << std::format("Final value of safe counter: {}", counter.load()) << std::endl;
    }
    
    void run() {
        run_unsafe_threads();
        run_safe_threads();
    }
};

int main() {
    Counter counter;
    counter.run();
}
#include <iostream>
#include <thread>
#include <atomic>
#include <format>
#include <vector>
#include <chrono>

class ThreadFactory {
public:
    template<typename Callable>
    static std::jthread createThread(Callable&& callable) {
        return std::jthread(std::forward<Callable>(callable));
    }
};

class Counter {
public:
    static void runUnsafeThreads() {
        int operation_count{100000000};
        int counter{0};

        auto incrementator = [&]() {
            for (int i = 0; i < operation_count; ++i)
                counter++;
        };

        auto decrementator = [&]() {
            for (int i = 0; i < operation_count; ++i)
                counter--;
        };

        auto start = std::chrono::high_resolution_clock::now();
        std::cout << std::format("Initial value of unsafe counter: {}", counter) << std::endl;
        auto incrementing_thread = ThreadFactory::createThread(incrementator);
        auto decrementing_thread = ThreadFactory::createThread(decrementator);

        incrementing_thread.join();
        decrementing_thread.join();

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << std::format("Final value of unsafe counter: {} (Duration: {} ms)", counter, duration) << std::endl;
    }
    static void runSafeThreads() {
        int operation_count{100000000};
        std::atomic<int> counter{0};

        auto incrementator = [&]() {
            for (int i = 0; i < operation_count; ++i)
                counter++;
        };

        auto decrementator = [&]() {
            for (int i = 0; i < operation_count; ++i)
                counter--;
        };

        auto start = std::chrono::high_resolution_clock::now();
        std::cout << std::format("Initial value of safe counter: {}", counter.load()) << std::endl;
        auto incrementing_thread = ThreadFactory::createThread(incrementator);
        auto decrementing_thread = ThreadFactory::createThread(decrementator);

        incrementing_thread.join();
        decrementing_thread.join();

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << std::format("Final value of safe counter: {} (Duration: {} ms)", counter.load(), duration) << std::endl;
    }
};
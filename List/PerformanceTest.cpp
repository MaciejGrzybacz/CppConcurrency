// PerformanceTest.cpp
#include <gtest/gtest.h>
#include <thread>
#include <vector>
#include <chrono>
#include <random>
#include "src/CoarseGrainList.hpp"
#include "src/FineGrainList.hpp"

template<typename ListType>
class ConcurrentListTest : public testing::Test {
protected:
    ListType list;
    const int NUM_THREADS = 4;
    const int OPERATIONS_PER_THREAD = 10;
};

using ListTypes = testing::Types<CoarseList<int>, FineList<int>>;
TYPED_TEST_SUITE(ConcurrentListTest, ListTypes);

TYPED_TEST(ConcurrentListTest, ParallelAdd) {
    std::vector<std::thread> threads;
    auto start = std::chrono::high_resolution_clock::now();

    for (int t = 0; t < this->NUM_THREADS; ++t) {
        threads.emplace_back([&, t]() {
            for (int i = 0; i < this->OPERATIONS_PER_THREAD; ++i) {
                int val = t * this->OPERATIONS_PER_THREAD + i;
                EXPECT_TRUE(this->list.add(val));
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "ParallelAdd " << typeid(TypeParam).name()
              << " took: " << duration.count() << "ms\n";

    // Verify all elements were added
    for (int t = 0; t < this->NUM_THREADS; ++t) {
        for (int i = 0; i < this->OPERATIONS_PER_THREAD; ++i) {
            int val = t * this->OPERATIONS_PER_THREAD + i;
            EXPECT_TRUE(this->list.contains(val));
        }
    }
}

TYPED_TEST(ConcurrentListTest, ParallelRemove) {
    // First add elements
    for (int i = 0; i < this->NUM_THREADS * this->OPERATIONS_PER_THREAD; ++i) {
        EXPECT_TRUE(this->list.add(i));
    }

    std::vector<std::thread> threads;
    auto start = std::chrono::high_resolution_clock::now();

    for (int t = 0; t < this->NUM_THREADS; ++t) {
        threads.emplace_back([&, t]() {
            for (int i = t; i < this->NUM_THREADS * this->OPERATIONS_PER_THREAD; i += this->NUM_THREADS) {
                EXPECT_TRUE(this->list.remove(i));
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "ParallelRemove " << typeid(TypeParam).name()
              << " took: " << duration.count() << "ms\n";

    // Verify all elements were removed
    for (int i = 0; i < this->NUM_THREADS * this->OPERATIONS_PER_THREAD; ++i) {
        EXPECT_FALSE(this->list.contains(i));
    }
}

TYPED_TEST(ConcurrentListTest, MixedOperations) {
    std::vector<std::thread> threads;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000);
    auto start = std::chrono::high_resolution_clock::now();

    // Add threads
    for (int t = 0; t < this->NUM_THREADS / 3; ++t) {
        threads.emplace_back([&]() {
            for (int i = 0; i < this->OPERATIONS_PER_THREAD; ++i) {
                this->list.add(dis(gen));
            }
        });
    }

    // Remove threads
    for (int t = 0; t < this->NUM_THREADS / 3; ++t) {
        threads.emplace_back([&]() {
            for (int i = 0; i < this->OPERATIONS_PER_THREAD; ++i) {
                this->list.remove(dis(gen));
            }
        });
    }

    // Contains threads
    for (int t = 0; t < this->NUM_THREADS / 3; ++t) {
        threads.emplace_back([&]() {
            for (int i = 0; i < this->OPERATIONS_PER_THREAD; ++i) {
                this->list.contains(dis(gen));
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "MixedOperations " << typeid(TypeParam).name()
              << " took: " << duration.count() << "ms\n";
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
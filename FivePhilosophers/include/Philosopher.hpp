#include <thread>
#include <shared_mutex>
#include <chrono>
#include <iostream>
#include <random>
#include <numeric>

enum class PhilosopherType {
    Naive,
    Starving,
    Asymmetric,
    Arbiter
};

class Philosopher {
public:
    Philosopher(std::shared_mutex &m1, std::shared_mutex &m2, int id)
            : left_fork(m1), right_fork(m2), id(id), rng(std::mt19937(std::random_device{}())),
              dist(std::uniform_int_distribution<>(1, 10)) {}

    virtual ~Philosopher() = default;

    virtual void eat() = 0;

    void think();

    void run();

    void print_waiting_stats() const;

    [[nodiscard]] double get_average_waiting_time() const;

protected:

    void record_waiting_time(const std::chrono::time_point<std::chrono::steady_clock>& start_time);

    void simulate_eating();

    int id;
    std::shared_mutex& left_fork;
    std::shared_mutex& right_fork;
    std::mt19937 rng;
    std::uniform_int_distribution<> dist;
    std::vector<long long> waiting_times;
};
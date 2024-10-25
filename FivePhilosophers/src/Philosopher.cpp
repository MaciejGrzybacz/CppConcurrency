#include "Philosopher.hpp"

void Philosopher::think()  {
    int sleep_time = dist(rng);
    std::cout << std::format("Philosopher {} is thinking for {} milliseconds \n", id, sleep_time);
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
}

void Philosopher::run()  {
    for (int i = 0; i < 10; ++i) {
        eat();
        think();
    }
    print_waiting_stats();
}

void Philosopher::print_waiting_stats() const  {
    if (waiting_times.empty()) {
        std::cout << std::format("Philosopher {} hasn't waited for forks yet or didn't finished waiting\n", id);
        return;
    }

    double avg_wait = std::accumulate(waiting_times.begin(), waiting_times.end(), 0.0)
                      / waiting_times.size();

    auto [min_wait, max_wait] = std::minmax_element(waiting_times.begin(), waiting_times.end());

    std::vector<long long> sorted_times = waiting_times;
    std::sort(sorted_times.begin(), sorted_times.end());
    double median_wait = sorted_times.size() % 2 == 0
                         ? (sorted_times[sorted_times.size() / 2 - 1] + sorted_times[sorted_times.size() / 2]) / 2.0
                         : sorted_times[sorted_times.size() / 2];

    std::cout << std::format(
            "Philosopher {} waiting stats:\n"
            "  Average wait: {:.2f} ms\n"
            "  Median wait: {:.2f} ms\n"
            "  Min wait: {} ms\n"
            "  Max wait: {} ms\n"
            "  Total attempts: {}\n",
            id, avg_wait, median_wait, *min_wait, *max_wait, waiting_times.size()
    );
}

double Philosopher::get_average_waiting_time() const  {
    if (waiting_times.empty()) return 0.0;
    return std::accumulate(waiting_times.begin(), waiting_times.end(), 0.0)
           / waiting_times.size();
}

void Philosopher::record_waiting_time(const std::chrono::time_point<std::chrono::steady_clock>& start_time) {
    auto end_wait = std::chrono::steady_clock::now();
    auto wait_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_wait - start_time);
    waiting_times.push_back(wait_time.count());
}


void Philosopher::simulate_eating() {
    int sleep_time = dist(rng);
    std::cout << std::format("Philosopher {} is eating for {} milliseconds\n", id, sleep_time);
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
}
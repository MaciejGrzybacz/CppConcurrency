#include <iostream>
#include <thread>
#include <condition_variable>
#include <deque>

class SharedData {
public:
    void push(int item) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return !ready; });
        buffer.push_back(item);
        ready = true;
        std::cout << "Produced: " << item << std::endl;
        lock.unlock();
        cv.notify_all();
    }

    int pop() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return ready; });
        int item = buffer.front();
        buffer.pop_front();
        ready = false;
        std::cout << "Consumed: " << item << std::endl;
        cv.notify_all();
        return item;
    }

private:
    std::deque<int> buffer;
    std::mutex mtx;
    std::condition_variable cv;
    bool ready = false;
};

void producer(SharedData& sharedData) {
    for (int i = 0; i < 10; ++i) {
        sharedData.push(i);
    }
}

void consumer(SharedData& sharedData) {
    for (int i = 0; i < 10; ++i) {
        sharedData.pop();
    }
}

void runOneToOne() {
    SharedData sharedData;

    std::jthread producerThread(producer, std::ref(sharedData));
    std::jthread consumerThread(consumer, std::ref(sharedData));
}

void runOneToMany() {
    SharedData sharedData;

    std::jthread producerThread(producer, std::ref(sharedData));
    std::jthread consumerOneThread(consumer, std::ref(sharedData));
    std::jthread consumerTwoThread(consumer, std::ref(sharedData));
}

void runManyToOne() {
    SharedData sharedData;

    std::jthread producerOneThread(producer, std::ref(sharedData));
    std::jthread producerTwoThread(producer, std::ref(sharedData));
    std::jthread consumerThread(consumer, std::ref(sharedData));
}

void runManyToMany() {
    SharedData sharedData;

    std::jthread producerOneThread(producer, std::ref(sharedData));
    std::jthread producerTwoThread(producer, std::ref(sharedData));
    std::jthread consumerOneThread(consumer, std::ref(sharedData));
    std::jthread consumerTwoThread(consumer, std::ref(sharedData));
}
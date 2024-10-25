#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <format>
#include <valarray>

class Buffer {
public:
    explicit Buffer(size_t size = 10) : max_size(size) {}
    
    void push(int val) {
        std::unique_lock<std::mutex> lck(mtx);
        notFull.wait(lck, [this]{return data.size() < max_size;});
        data.push(val);
        notEmpty.notify_all();  
    } 
    
    int get() {
        std::unique_lock<std::mutex> lck(mtx);
        notEmpty.wait(lck, [this] { return !data.empty();});
        int val = data.front();
        data.pop();
        notFull.notify_all();
        return val;
    }
    
private:
    std::queue<int> data;
    size_t max_size;
    std::mutex mtx;
    std::condition_variable notFull, notEmpty;
};

class Process {
    virtual void process(int data) = 0;
public:
    virtual ~Process() = default;
};

class Producer : public Process {
public:
    explicit Producer(Buffer& buff) : buffer(buff) {};
    
    void process(int data) override {
        buffer.push(data);
    }
    
    void run() {
        for (int i = 0; i < 3; ++i) {
            int val = pow(10,i);
            process(val);
            std::cout << std::format("Produced {} \n", val);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
    
private:
    Buffer& buffer;
};

class Processor : public Process {
public:
    explicit Processor(Buffer& in, Buffer& out) : input(in), output(out) {};
    
    void process(int val) override {
        val *= 2;
        output.push(val);
    }
    
    void run() {
        for (int i = 0; i < 3; ++i) {
            int val = input.get();
            process(val);
        }
    }
    
private:
    Buffer& input;
    Buffer& output;
};

class Consumer : public Process {
public:
    explicit Consumer(Buffer& out) : input(out) {};
    
    void process(int val) override {
        std::cout << std::format("Consumed {} \n", val);
    }
    
    void run() {
        for (int i = 0; i < 3; ++i) {
            int val = input.get();
            process(val);
        }
    }
    
private:
    Buffer& input;
};

#include <thread>

class Simulation {
    Buffer inputBuffer, midBuffer1, midBuffer2, outputBuffer;
    Producer producer;
    Processor processor1;
    Processor processor2;
    Processor processor3;
    Consumer consumer;

public:
    Simulation()
            : producer(inputBuffer),
              processor1(inputBuffer, midBuffer1),
              processor2(midBuffer1, midBuffer2),
              processor3(midBuffer2, outputBuffer),
              consumer(outputBuffer) {}

    void run() {
        std::thread producerThread(&Producer::run, &producer);
        std::thread processor1Thread(&Processor::run, &processor1);
        std::thread processor2Thread(&Processor::run, &processor2);
        std::thread processor3Thread(&Processor::run, &processor3);
        std::thread consumerThread(&Consumer::run, &consumer);

        producerThread.join();
        processor1Thread.join();
        processor2Thread.join();
        processor3Thread.join();
        consumerThread.join();
    }
};
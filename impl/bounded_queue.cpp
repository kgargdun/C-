
#include<iostream>
#include<queue>
#include<mutex>
#include<condition_variable>
#include<thread>


class BoundedQueue {
public:
    BoundedQueue(int capacity) : capacity(capacity) {}

    void push(int value) {
        std::unique_lock<std::mutex> lock(mtx);
        canPush.wait(lock, [this]() { return buffer.size() < capacity; });
        buffer.push(value);
        std::cout << "Pushed: " << value << std::endl;
        canPop.notify_one();
    }

    int pop() {
        std::unique_lock<std::mutex>lock(mtx);
        canPop.wait(lock, [this]() { return !buffer.empty(); });
        int value = buffer.front();
        buffer.pop();
        canPush.notify_one();
        std::cout << "Popped: " << value << std::endl;
        return value;
    }

private:
    int capacity;
    std::queue<int> buffer;
    std::mutex mtx;
    std::condition_variable canPush;
    std::condition_variable canPop;
};

int main() {

    BoundedQueue boundedQueue(5);

    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    for(int i=0;i<10;i++) {
        producers.emplace_back([&boundedQueue, i](){
            boundedQueue.push(i);
        });
    }

    for(int i=0;i<10;i++) {
        consumers.emplace_back([&boundedQueue](){
            int x = boundedQueue.pop();
        });
    }

    for(auto& producer : producers) {
        producer.join();
    }

    for(auto& consumer : consumers) {
        consumer.join();
    }


}
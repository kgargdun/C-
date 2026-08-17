

#include<iostream>
#include<thread>
using namespace std;

vector<int> buffer;
std::atomic<bool>ready{false};

void producer(int x) {
    buffer.push_back(x);
    ready.store(true, std::memory_order_release);
}

void consumer() {
    while(!ready.load(std::memory_order_acquire)) {
        std::this_thread::yield();
    }
    std::cout<<"Consumed: "<<buffer.back()<<std::endl;
}

int main() {
    thread t2(consumer);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    thread t1(producer, 10);
    t1.join();
    t2.join();
}
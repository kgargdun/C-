#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

class SpinLock {
public:
    void lock() {
        bool expected = false;

        while (!locked.compare_exchange_weak(
            expected,
            true,
            std::memory_order_acquire,
            std::memory_order_relaxed
        )) {
            // CAS failure updates expected to the current value,
            // usually true. Reset it before retrying false -> true.
            expected = false;
            std::this_thread::yield();
        }
    }

    void unlock() {
        locked.store(false, std::memory_order_release);
    }

private:
    std::atomic<bool> locked{false};
};

SpinLock spinLock;
int counter = 0;

void worker() {
    for (int i = 0; i < 100'000; ++i) {
        std::lock_guard<SpinLock> guard(spinLock);
        ++counter;
    }
}

int main() {
    std::vector<std::thread> threads;

    for (int i = 0; i < 4; ++i) {
        threads.emplace_back(worker);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "Counter: " << counter << '\n';
}
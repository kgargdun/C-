#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>
#include <functional>

class ThreadPool {
public:
    using Task = std::function<void()>;

    ThreadPool(int count) {
        for (int i = 0; i < count; i++) {
            workers.emplace_back([this]() {
                while (true) {
                    Task task;

                    {
                        std::unique_lock<std::mutex> lock(mtx);

                        cv.wait(lock, [this]() {
                            return !tasks.empty() || stopping;
                        });

                        if (stopping && tasks.empty())
                            return;

                        task = std::move(tasks.front());
                        tasks.pop();
                    }

                    task(); // outside lock
                }
            });
        }
    }

    void submit(Task task) {
        {
            std::lock_guard<std::mutex> lock(mtx);

            if (stopping)
                return;

            tasks.push(std::move(task));
        }

        cv.notify_one();
    }

    void shutdown() {
        {
            std::lock_guard<std::mutex> lock(mtx);

            if (stopping)
                return;

            stopping = true;
        }

        cv.notify_all();

        for (auto& worker : workers) {
            if (worker.joinable())
                worker.join();
        }
    }

    ~ThreadPool() {
        shutdown();
    }

private:
    std::queue<Task> tasks;
    std::mutex mtx;
    std::condition_variable cv;
    std::vector<std::thread> workers;
    bool stopping = false;
};


int main() {
    ThreadPool pool(4);

    for (int i = 0; i < 10; ++i) {
        pool.submit([i]() {
            std::cout << "Task " << i << " is running in thread " 
                      << std::this_thread::get_id() << std::endl;
        });
    }

    pool.shutdown();

    return 0;
}
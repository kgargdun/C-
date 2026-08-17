
#include<iostream>
#include<unordered_map>
#include<mutex>
#include<shared_mutex>
#include<thread>


class Cache {
public:
    void put(int key ,int value) {
        std::unique_lock<std::shared_mutex>lock(mtx);
        cache[key] = value;
    }

    int get(int key) {
        std::shared_lock<std::shared_mutex>lock(mtx);
        auto it = cache.find(key);
        if(it != cache.end()) {
            return it->second;
        }
        return -1; // or throw an exception

    }

    void erase(int key) {
        std::unique_lock<std::shared_mutex>lock(mtx);
        cache.erase(key);
    }

private:
    std::unordered_map<int, int> cache;
    std::shared_mutex mtx;

};

int main() {

    Cache cache;

    std::vector<std::thread> writers;
    std::vector<std::thread> readers;

    for(int i=0;i<10;i++) {
        writers.emplace_back([&cache, i](){
            cache.put(i, i*10);
            std::cout << "Put: " << i << " -> " << i*10 << std::endl;
        });
    }

    for(int i=0;i<10;i++) {
        readers.emplace_back([&cache, i](){
            int value = cache.get(i);
            std::cout << "Get: " << i << " -> " << value << std::endl;
        });
    }

    for(auto& writer : writers) {
        writer.join();
    }

    for(auto& reader : readers) {
        reader.join();
    }


}
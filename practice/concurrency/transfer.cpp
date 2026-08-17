#include <iostream>
#include <mutex>
#include <thread>

class BankAccount {
public:
    explicit BankAccount(int balance) : balance(balance) {}

    int getBalance() {
        std::lock_guard<std::mutex> lock(mtx);
        return balance;
    }

    void transferTo(BankAccount& other, int amount) {
        if (this == &other || amount <= 0)
            return;

        // Locks both mutexes without deadlock.
        std::scoped_lock lock(mtx, other.mtx);

        if (balance < amount)
            return;

        balance -= amount;
        other.balance += amount;
    }

private:
    int balance;
    std::mutex mtx;
};

int main() {
    BankAccount a(1000);
    BankAccount b(1000);

    std::thread t1([&]() {
        for (int i = 0; i < 100; ++i)
            a.transferTo(b, 10);
    });

    std::thread t2([&]() {
        for (int i = 0; i < 100; ++i)
            b.transferTo(a, 10);
    });

    t1.join();
    t2.join();

    std::cout << "A: " << a.getBalance() << '\n';
    std::cout << "B: " << b.getBalance() << '\n';
}
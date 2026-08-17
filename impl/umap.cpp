#include <cstddef>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>

template<
    typename Key,
    typename Value,
    typename Hasher = std::hash<Key>,
    typename EqualTo = std::equal_to<Key>
>
class HashTable {
private:
    enum class Status {
        EMPTY,
        TAKEN,
        DELETED
    };

    struct Node {
        Key key;
        Value value;
        Status status;
    };

    std::size_t capacity_;
    std::size_t size_;
    Node* buffer_;

    Hasher hasher_{};
    EqualTo isEqual_{};

    Node* findCandidateNode(const Key& key) {
        std::size_t start = hasher_(key) % capacity_;
        Node* firstDeleted = nullptr;

        for (std::size_t i = 0; i < capacity_; ++i) {
            std::size_t slot = (start + i) % capacity_;
            Node& node = buffer_[slot];

            if (node.status == Status::DELETED) {
                if (!firstDeleted) {
                    firstDeleted = &node;
                }
                continue;
            }

            if (node.status == Status::EMPTY) {
                return firstDeleted ? firstDeleted : &node;
            }

            if (isEqual_(key, node.key)) {
                return &node;
            }
        }

        return firstDeleted;
    }

public:
    explicit HashTable(std::size_t capacity = 8)
        : capacity_(capacity == 0 ? 1 : capacity),
          size_(0),
          buffer_(new Node[capacity_]) {

        for (std::size_t i = 0; i < capacity_; ++i) {
            buffer_[i].status = Status::EMPTY;
        }
    }

    ~HashTable() {
        delete[] buffer_;
    }

    bool insert_or_assign(const Key& key, const Value& value) {
        Node* candidate = findCandidateNode(key);

        if (!candidate) {
            throw std::overflow_error("HashTable is full");
        }

        if (candidate->status == Status::TAKEN) {
            candidate->value = value;
            return false;
        }

        candidate->key = key;
        candidate->value = value;
        candidate->status = Status::TAKEN;
        ++size_;

        return true;
    }

    bool erase(const Key& key) {
        Node* candidate = findCandidateNode(key);

        if (!candidate || candidate->status != Status::TAKEN) {
            return false;
        }

        candidate->status = Status::DELETED;
        --size_;
        return true;
    }

    Value& operator[](const Key& key) {
        Node* candidate = findCandidateNode(key);

        if (!candidate) {
            throw std::overflow_error("HashTable is full");
        }

        if (candidate->status != Status::TAKEN) {
            candidate->key = key;
            candidate->value = Value{};
            candidate->status = Status::TAKEN;
            ++size_;
        }

        return candidate->value;
    }
};

struct ConstantHash {
    std::size_t operator()(int) const {
        return 0; // Force collisions for testing.
    }
};

int main() {
    HashTable<int, std::string, ConstantHash> table(4);

    std::cout << std::boolalpha;

    std::cout << table.insert_or_assign(1, "one") << '\n';   // true
    std::cout << table.insert_or_assign(2, "two") << '\n';   // true
    std::cout << table.insert_or_assign(2, "TWO") << '\n';   // false

    std::cout << table[2] << '\n';                           // TWO
    std::cout << table.erase(1) << '\n';                     // true
    std::cout << table.erase(10) << '\n';                    // false

    table[3] = "three"; // Reuses the deleted slot.
    std::cout << table[3] << '\n';                           // three

    return 0;
}
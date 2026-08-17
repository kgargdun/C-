
#include<iostream>

template<typename T>
class Shared_Ptr {
public:

    Shared_Ptr (T *inData) {
        data = inData;
        refCount = new int{1};
    }
    
    Shared_Ptr() {
        data = nullptr;
        refCount = nullptr;
    }
    
    Shared_Ptr (const Shared_Ptr& other) {
        data = other.get();
        refCount = other.getRef();
        if(refCount) (*refCount)++;
    }
    
    Shared_Ptr& operator=(const Shared_Ptr& other) {
        if(this != &other) {
            release();
            data = other.get();
            refCount = other.getRef();
            if(refCount) (*refCount)++;
        }
        return *this;
    }
    
    Shared_Ptr (Shared_Ptr &&other) noexcept {
        data = other.get();
        refCount = other.getRef();
        other.data = nullptr;
        other.refCount = nullptr;
    }
    
    Shared_Ptr& operator=(Shared_Ptr &&other) noexcept {
        if(this != &other) {
            release();
            data = other.get();
            refCount = other.getRef();
            other.data = nullptr;
            other.refCount = nullptr;
        }
        return *this;
    }
    
    
    T* get() const {
        return data;
    }
    
    void reset() {
        release();
        data = nullptr;
        refCount = nullptr;
    }
    
    
    int use_count() const {
        return refCount == nullptr ? 0 : *refCount;
    }
    
    T* operator->() {
        return data;
    }
    
    T& operator*() {
        return *data;
    }
    
    ~Shared_Ptr() {
        release();
        data = nullptr;
        refCount = nullptr;
    }
    
    
private:
    
    int* getRef() const {
        return refCount;
    }
    
    void release() {
        
        if(refCount == nullptr) return;
        (*refCount)--;
        if(*refCount == 0) {
            delete data;
            delete refCount;
            data = nullptr;
            refCount = nullptr;
        }
    }
    
    T *data;
    int *refCount;
};

template<typename T>
Shared_Ptr<T> Make_Shared(T val) {
    return Shared_Ptr<T>(new T{val});
}

int main() {

    // Basic construction
    Shared_Ptr<int> p1 = Make_Shared<int>(10);

    std::cout << *p1 << "\n";            // 10
    std::cout << p1.use_count() << "\n"; // 1


    // Copy constructor
    Shared_Ptr<int> p2(p1);

    std::cout << p1.use_count() << "\n"; // 2
    std::cout << p2.use_count() << "\n"; // 2


    // Copy assignment
    Shared_Ptr<int> p3;
    p3 = p1;

    std::cout << p1.use_count() << "\n"; // 3
    std::cout << p3.use_count() << "\n"; // 3


    // Reset
    p2.reset();

    std::cout << p2.use_count() << "\n"; // 0
    std::cout << p1.use_count() << "\n"; // 2


    // Move constructor
    Shared_Ptr<int> p4(std::move(p3));

    std::cout << p3.use_count() << "\n"; // 0
    std::cout << p4.use_count() << "\n"; // 2


    // Move assignment
    Shared_Ptr<int> p5;
    p5 = std::move(p4);

    std::cout << p4.use_count() << "\n"; // 0
    std::cout << p5.use_count() << "\n"; // 2

    std::cout << *p5 << "\n";            // 10

    return 0;
}

#include<iostream>
using namespace std;


template<typename T, size_t N>
class Array {
public:
    T &operator[] (size_t index) {
        return data[index];
    }
    size_t size() const {
        return N;
    }
private:
    T data[N];
};

int main() {

    Array<int,5> arr;
    for(int i=0;i<arr.size();i++) {
        arr[i] = i*i;   
    }
    for(int i=0;i<arr.size();i++) {
        cout<<arr[i]<<" ";   
    }

}
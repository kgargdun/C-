

#include<iostream>
#include<thread>
#include<chrono>
#include<stop_token>


void work(std::stop_token st) {
    for(int i=0;i<100;i++) {
        if(st.stop_requested()) {
            std::cout<<"Stop requested, exiting thread."<<std::endl;
            return;
        }   
        std::cout<<"Wrking in thread: "<< i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}


int main() {

    std::jthread t(work);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

}


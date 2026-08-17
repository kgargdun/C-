

#include<iostream>

class Logger {
public:
    static Logger& getInstance()  {
        static Logger instance;
        return instance;
    }

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;


private:
    Logger() = default;
    ~Logger() = default;
};

int main() {

}
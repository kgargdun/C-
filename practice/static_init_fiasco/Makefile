CXX = g++
CXXFLAGS = -std=c++17 -O0 -g
TARGET = static_init_demo
SRCS = src/alpha.cpp src/beta.cpp src/main.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)

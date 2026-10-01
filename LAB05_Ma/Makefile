CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

TARGET = matrix

$(TARGET): main.o matrix.o
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.o matrix.o

main.o: main.cpp matrix.h
	$(CXX) $(CXXFLAGS) -c main.cpp

matrix.o: matrix.cpp matrix.h
	$(CXX) $(CXXFLAGS) -c matrix.cpp

clean:
	rm -f *.o $(TARGET)

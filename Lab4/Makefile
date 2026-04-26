CXX      = g++
CXXFLAGS = -g -Wall -std=c++17

OBJS = build/point.o build/circle.o build/list.o

.PHONY: run test clean

build/point.o: src/point.cpp src/point.h
	$(CXX) $(CXXFLAGS) -c -o build/point.o src/point.cpp

build/circle.o: src/circle.cpp src/circle.h src/point.h
	$(CXX) $(CXXFLAGS) -c -o build/circle.o src/circle.cpp

build/list.o: src/list.cpp src/list.h src/circle.h
	$(CXX) $(CXXFLAGS) -c -o build/list.o src/list.cpp

build/debug.out: $(OBJS) src/main.cpp
	$(CXX) $(CXXFLAGS) -o build/debug.out src/main.cpp $(OBJS)

build/test.out: $(OBJS) tests/test_lab4.cpp
	$(CXX) $(CXXFLAGS) -o build/test.out tests/test_lab4.cpp $(OBJS)

run: build/debug.out
	./build/debug.out

test: build/test.out
	./build/test.out

clean:
	rm -f build/*.o build/*.out

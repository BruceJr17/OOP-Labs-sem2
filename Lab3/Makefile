CXX      = g++
CXXFLAGS = -g -Wall -std=c++17

OBJS = build/my_string.o build/matrix.o build/worker_db.o \
       build/notification.o build/base_file.o build/bool_array.o

.PHONY: run test clean

build/my_string.o: src/my_string.cpp src/my_string.h
	$(CXX) $(CXXFLAGS) -c -o build/my_string.o src/my_string.cpp

build/matrix.o: src/matrix.cpp src/matrix.h
	$(CXX) $(CXXFLAGS) -c -o build/matrix.o src/matrix.cpp

build/worker_db.o: src/worker_db.cpp src/worker_db.h src/my_string.h
	$(CXX) $(CXXFLAGS) -c -o build/worker_db.o src/worker_db.cpp

build/notification.o: src/notification.cpp src/notification.h
	$(CXX) $(CXXFLAGS) -c -o build/notification.o src/notification.cpp

build/base_file.o: src/base_file.cpp src/base_file.h
	$(CXX) $(CXXFLAGS) -c -o build/base_file.o src/base_file.cpp

build/bool_array.o: src/bool_array.cpp src/bool_array.h
	$(CXX) $(CXXFLAGS) -c -o build/bool_array.o src/bool_array.cpp

build/debug.out: $(OBJS) src/main.cpp
	$(CXX) $(CXXFLAGS) -o build/debug.out src/main.cpp $(OBJS)

build/test.out: $(OBJS) tests/test_lab3.cpp
	$(CXX) $(CXXFLAGS) -o build/test.out tests/test_lab3.cpp $(OBJS)

run: build/debug.out
	./build/debug.out

test: build/test.out
	./build/test.out

clean:
	rm -f build/*.o build/*.out

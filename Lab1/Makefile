
# Task 3.5 improvements:
# - CXX and CXXFLAGS variables: change compiler or flags in one place
# - OBJS variable: list object files once, reuse everywhere
# - .PHONY: prevents issues if a file named 'clean' or 'test' ever exists
# - clean target: wipes all build output to start fresh

CXX      = g++
CXXFLAGS = -g -Wall -std=c++17

OBJS = build/rect.o build/barrel.o build/matrix.o \
	     build/my_string.o build/text_wrapper.o

.PHONY: run debug leaks test clean

build/rect.o: src/rect.cpp src/rect.h
	$(CXX) $(CXXFLAGS) -c -o build/rect.o src/rect.cpp

build/barrel.o: src/barrel.cpp src/barrel.h
	$(CXX) $(CXXFLAGS) -c -o build/barrel.o src/barrel.cpp

build/matrix.o: src/matrix.cpp src/matrix.h
	$(CXX) $(CXXFLAGS) -c -o build/matrix.o src/matrix.cpp

build/my_string.o: src/my_string.cpp src/my_string.h
	$(CXX) $(CXXFLAGS) -c -o build/my_string.o src/my_string.cpp

build/text_wrapper.o: src/text_wrapper.cpp src/text_wrapper.h src/my_string.h
	$(CXX) $(CXXFLAGS) -c -o build/text_wrapper.o src/text_wrapper.cpp

build/debug.out: $(OBJS) src/main.cpp
	$(CXX) $(CXXFLAGS) -o build/debug.out src/main.cpp $(OBJS)

run: build/debug.out
	./build/debug.out

build/leaks.out: $(OBJS) src/main.cpp
	$(CXX) $(CXXFLAGS) -fsanitize=address -o build/leaks.out src/main.cpp $(OBJS)

leaks: build/leaks.out
	./build/leaks.out < tests/input.txt

build/test_rect_basic_methods.out: build/rect.o tests/test_rect_basic_methods.cpp
	$(CXX) $(CXXFLAGS) -o build/test_rect_basic_methods.out tests/test_rect_basic_methods.cpp build/rect.o

build/test_rect_properties.out: build/rect.o tests/test_rect_properties.cpp
	$(CXX) $(CXXFLAGS) -o build/test_rect_properties.out tests/test_rect_properties.cpp build/rect.o

build/test_rect_operations.out: build/rect.o tests/test_rect_operations.cpp
	$(CXX) $(CXXFLAGS) -o build/test_rect_operations.out tests/test_rect_operations.cpp build/rect.o

build/test_bounding_rect.out: build/rect.o tests/test_bounding_rect.cpp
	$(CXX) $(CXXFLAGS) -o build/test_bounding_rect.out tests/test_bounding_rect.cpp build/rect.o

test: build/test_rect_basic_methods.out \
	    build/test_rect_properties.out    \
	    build/test_rect_operations.out    \
	    build/test_bounding_rect.out
	./build/test_rect_basic_methods.out
	./build/test_rect_properties.out
	./build/test_rect_operations.out
	./build/test_bounding_rect.out
	@echo "All tests passed!"

clean:
	rm -f build/*.o build/*.out

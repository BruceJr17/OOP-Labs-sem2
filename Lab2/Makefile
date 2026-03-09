CXX      = g++
CXXFLAGS = -g -Wall -std=c++17

OBJS = build/my_string.o build/base_file.o build/base32_file.o \
       build/rle_file.o  build/composed_files.o

.PHONY: run debug leaks test clean

build/my_string.o: src/my_string.cpp src/my_string.h
	$(CXX) $(CXXFLAGS) -c -o build/my_string.o src/my_string.cpp

build/base_file.o: src/base_file.cpp src/base_file.h src/ifile.h
	$(CXX) $(CXXFLAGS) -c -o build/base_file.o src/base_file.cpp

build/base32_file.o: src/base32_file.cpp src/base32_file.h src/base_file.h
	$(CXX) $(CXXFLAGS) -c -o build/base32_file.o src/base32_file.cpp

build/rle_file.o: src/rle_file.cpp src/rle_file.h src/base_file.h
	$(CXX) $(CXXFLAGS) -c -o build/rle_file.o src/rle_file.cpp

build/composed_files.o: src/composed_files.cpp src/composed_files.h src/ifile.h
	$(CXX) $(CXXFLAGS) -c -o build/composed_files.o src/composed_files.cpp

build/debug.out: $(OBJS) src/main.cpp
	$(CXX) $(CXXFLAGS) -o build/debug.out src/main.cpp $(OBJS)

run: build/debug.out
	./build/debug.out

# ── unit tests ────────────────────────────────────────────────────────────────

build/test_base_file.out: $(OBJS) tests/test_base_file.cpp
	$(CXX) $(CXXFLAGS) -o build/test_base_file.out tests/test_base_file.cpp $(OBJS)

build/test_base32_file.out: $(OBJS) tests/test_base32_file.cpp
	$(CXX) $(CXXFLAGS) -o build/test_base32_file.out tests/test_base32_file.cpp $(OBJS)

build/test_rle_file.out: $(OBJS) tests/test_rle_file.cpp
	$(CXX) $(CXXFLAGS) -o build/test_rle_file.out tests/test_rle_file.cpp $(OBJS)

build/test_composed.out: $(OBJS) tests/test_composed.cpp
	$(CXX) $(CXXFLAGS) -o build/test_composed.out tests/test_composed.cpp $(OBJS)

test: build/test_base_file.out    \
      build/test_base32_file.out  \
      build/test_rle_file.out     \
      build/test_composed.out
	./build/test_base_file.out
	./build/test_base32_file.out
	./build/test_rle_file.out
	./build/test_composed.out
	@echo "All tests passed!"

# ── clean ─────────────────────────────────────────────────────────────────────

clean:
	rm -f build/*.o build/*.out build/*.bin build/*.txt

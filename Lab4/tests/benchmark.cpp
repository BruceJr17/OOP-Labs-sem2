#include <iostream>
#include <chrono>
#include <cstdlib>
#include "../src/list.h"
#include "../src/circle.h"
#include "../src/point.h"

void fill_random(List& l, int n) {
    l.clear();
    for (int i = 0; i < n; i++) {
        Circle c(Point(rand() % 100, rand() % 100), rand() % 100 + 1);
        l.push_back(c);
    }
}

double measure_ms(List l, void(List::*sort_fn)()) {
    auto start = std::chrono::high_resolution_clock::now();
    (l.*sort_fn)();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

int main() {
    srand(42);
    const int RUNS = 5;
    int sizes[] = {1000, 10000, 100000};

    std::cout << "Size\t\tBubble\t\tInsertion\tInsert-Ptr\tMerge\n";
    std::cout << "-------------------------------------------------------------------\n";

    for (int sz : sizes) {
        double b=0, ins=0, ptr=0, mrg=0;
        for (int r = 0; r < RUNS; r++) {
            List base;
            fill_random(base, sz);
            b   += measure_ms(base, &List::sort_bubble);
            ins += measure_ms(base, &List::sort_by_area);
            ptr += measure_ms(base, &List::sort_insertion_ptr);
            mrg += measure_ms(base, &List::sort_merge);
        }
        std::cout << sz << "\t\t"
                  << b/RUNS   << "ms\t\t"
                  << ins/RUNS << "ms\t\t"
                  << ptr/RUNS << "ms\t\t"
                  << mrg/RUNS << "ms\n";
    }
    return 0;
}

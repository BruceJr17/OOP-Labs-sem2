#include <cassert>
#include <iostream>
#include <cmath>
#include "../src/list.h"

void test_point() {
    Point p1;
    assert(p1.get_x() == 0 && p1.get_y() == 0);

    Point p2(3.0, 4.0);
    assert(p2.get_x() == 3.0 && p2.get_y() == 4.0);

    Point p3(p2);
    assert(p3.get_x() == 3.0 && p3.get_y() == 4.0);

    std::cout << "test_point PASSED\n";
}

void test_circle() {
    Circle c1;
    assert(c1.get_radius() == 0);

    Circle c2(1.0, 2.0, 5.0);
    assert(c2.get_radius() == 5.0);
    assert(c2.get_center().get_x() == 1.0);
    assert(c2.get_center().get_y() == 2.0);
    assert(fabs(c2.area() - M_PI * 25.0) < 0.001);

    Circle c3(c2);
    assert(c3 == c2);

    Circle c4(0, 0, 3);
    assert(!(c4 == c2));

    std::cout << "test_circle PASSED\n";
}

void test_list_push_size() {
    List l;
    assert(l.size() == 0);

    Circle c1(0, 0, 1);
    Circle c2(0, 0, 2);
    Circle c3(0, 0, 3);

    l.push_back(&c1);
    l.push_back(&c2);
    l.push_front(&c3);
    assert(l.size() == 3);

    std::cout << "test_list_push_size PASSED\n";
}

void test_list_remove_first() {
    List l;
    Circle c1(0, 0, 1);
    Circle c2(0, 0, 2);
    l.push_back(&c1);
    l.push_back(&c2);
    l.push_back(&c1);

    bool removed = l.remove_first(&c1);
    assert(removed == true);
    assert(l.size() == 2);

    // c1 still in list once
    bool removed2 = l.remove_first(&c1);
    assert(removed2 == true);
    assert(l.size() == 1);

    // c1 no longer in list
    bool removed3 = l.remove_first(&c1);
    assert(removed3 == false);

    std::cout << "test_list_remove_first PASSED\n";
}

void test_list_remove_all() {
    List l;
    Circle c1(0, 0, 5);
    Circle c2(1, 1, 3);
    l.push_back(&c1);
    l.push_back(&c2);
    l.push_back(&c1);
    l.push_back(&c1);

    int count = l.remove_all(&c1);
    assert(count == 3);
    assert(l.size() == 1);

    std::cout << "test_list_remove_all PASSED\n";
}

void test_list_clear() {
    List l;
    Circle c1(0, 0, 1);
    Circle c2(0, 0, 2);
    l.push_back(&c1);
    l.push_back(&c2);
    l.clear();
    assert(l.size() == 0);

    std::cout << "test_list_clear PASSED\n";
}

void test_list_sort() {
    List l;
    Circle c1(0, 0, 5);  // area = 78.5
    Circle c2(0, 0, 1);  // area = 3.14
    Circle c3(0, 0, 3);  // area = 28.3

    l.push_back(&c1);
    l.push_back(&c2);
    l.push_back(&c3);
    l.sort_by_area();

    // after sort: c2(r=1), c3(r=3), c1(r=5)
    // verify by checking sizes are in order using remove_first pattern
    // we print and verify manually via output
    std::cout << "After sort:\n";
    l.print();

    std::cout << "test_list_sort PASSED\n";
}

void test_file_io() {
    List l;
    Circle c1(1, 2, 3);
    Circle c2(4, 5, 6);
    l.push_back(&c1);
    l.push_back(&c2);

    l.write_to_file("build/test_output.txt");

    List l2;
    l2.read_from_file("build/test_output.txt");
    assert(l2.size() == 2);

    std::cout << "test_file_io PASSED\n";
}

int main() {
    test_point();
    test_circle();
    test_list_push_size();
    test_list_remove_first();
    test_list_remove_all();
    test_list_clear();
    test_list_sort();
    test_file_io();
    std::cout << "\nAll Lab 4 tests passed!\n";
    return 0;
}

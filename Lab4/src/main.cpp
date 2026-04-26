#include <iostream>
#include <fstream>
#include "list.h"
using namespace std;

int main() {
    cout << "Lab 4 starting...\n\n";

    List l;

    Circle c1(0, 0, 5);
    Circle c2(1, 2, 3);
    Circle c3(4, 4, 7);
    Circle c4(0, 0, 5);  // duplicate of c1

    l.push_back(&c1);
    l.push_back(&c2);
    l.push_back(&c3);
    l.push_front(&c4);

    cout << "--- Initial list ---\n";
    cout << l;

    cout << "--- After sort by area ---\n";
    l.sort_by_area();
    cout << l;

    cout << "--- Remove first circle with r=5 ---\n";
    bool removed = l.remove_first(&c1);
    cout << "Removed: " << removed << "\n";
    cout << l;

    cout << "--- Remove all circles with r=5 ---\n";
    l.push_back(&c1);
    l.push_back(&c4);
    int count = l.remove_all(&c1);
    cout << "Removed count: " << count << "\n";
    cout << l;

    cout << "--- Write to file ---\n";
    l.write_to_file("build/output.txt");
    cout << "Written to build/output.txt\n";

    cout << "--- Read from file ---\n";
    List l2;
    l2.read_from_file("build/output.txt");
    cout << l2;
    return 0;
}

#include "point.h"
#include <iostream>

Point::Point() : x(0), y(0) {}

Point::Point(double x, double y) : x(x), y(y) {}

Point::Point(const Point& other) : x(other.x), y(other.y) {}

void Point::print() const {
    std::cout << "(" << x << ", " << y << ")";
}

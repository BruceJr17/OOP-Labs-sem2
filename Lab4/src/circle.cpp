#include "circle.h"
#include <iostream>
#include <cmath>

Circle::Circle() : center(0, 0), radius(0) {}

Circle::Circle(double x, double y, double r) : center(x, y), radius(r) {}

Circle::Circle(const Point& c, double r) : center(c), radius(r) {}

Circle::Circle(const Circle& other) : center(other.center), radius(other.radius) {}

double Circle::area() const {
    return M_PI * radius * radius;
}

bool Circle::operator==(const Circle& other) const {
    return center.get_x() == other.center.get_x() &&
           center.get_y() == other.center.get_y() &&
           radius == other.radius;
}

void Circle::print() const {
    std::cout << "Circle(center=";
    center.print();
    std::cout << ", r=" << radius << ", area=" << area() << ")";
}

std::ostream& operator<<(std::ostream& os, const Circle& c) {
    os << c.center.get_x() << " "
       << c.center.get_y() << " "
       << c.radius;
    return os;
}

std::istream& operator>>(std::istream& is, Circle& c) {
    double x, y, r;
    is >> x >> y >> r;
    c = Circle(x, y, r);
    return is;
}



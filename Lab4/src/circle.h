#pragma once
#include "point.h"
#include <iostream>

class Circle {
private:
    Point center;
    double radius;
public:
    Circle();
    Circle(double x, double y, double r);
    Circle(const Point& center, double r);
    Circle(const Circle& other);

    const Point& get_center() const { return center; }
    double get_radius() const { return radius; }
    double area() const;

    bool operator==(const Circle& other) const;
    void print() const;

    friend std::ostream& operator<<(std::ostream& os, const Circle& c);
    friend std::istream& operator>>(std::istream& is, Circle& c);

};

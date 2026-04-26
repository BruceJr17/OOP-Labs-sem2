#pragma once

class Point {
private:
    double x;
    double y;
public:
    Point();
    Point(double x, double y);
    Point(const Point& other);

    double get_x() const { return x; }
    double get_y() const { return y; }
    void set_x(double x) { this->x = x; }
    void set_y(double y) { this->y = y; }

    void print() const;
};

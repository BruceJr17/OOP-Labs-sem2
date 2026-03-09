#pragma once
#include <iostream>

class Rect {
private:
     // Task 5: changed from storing {left, right, top, bottom} directly
    // to storing bottom-left corner + dimensions {x, y, width, height}.
    // The public interface stayed exactly the same.
    // Only the constructor, set_normalised, and getters changed internally.

    int x;      // x-coordinate of the left edge   (was: left)
    int y;      // y-coordinate of the bottom edge (was: bottom)
    int width;  // horizontal size                 (was: right - left)
    int height; // vertical size                   (was: top - bottom)

    void set_normalised(int left, int right, int top, int bottom);

public:
    // Default constructor - required once we define an explicit constructor
    Rect();

    // Parameterized constructor: left, right, top, bottom edges
    Rect(int left, int right, int top, int bottom);

    // Copy constructor (Task 1.6)
    Rect(const Rect& other);

    // Destructor (Task 1.5)
    ~Rect();

    // Getters
    int get_left()   const;
    int get_right()  const;
    int get_top()    const;
    int get_bottom() const;

    // Computed properties: width, height, area (Task 1.11)
    int get_width()  const;
    int get_height() const;
    int get_square() const;  // area = width * height

    void set_width(int w);
    void set_height(int h);

    // Set all four edges at once
    void set_all(int left, int right, int top, int bottom);

    // inflate overloads (Task 1.9) - push each edge outward
    void inflate(int amount);                                          // all sides equally
    void inflate(int dw, int dh);                                     // separate x / y amounts
    void inflate(int d_left, int d_right, int d_top, int d_bottom);   // each side individually

    // Translate the rectangle (Task 1.9) - dy defaults to 0
    void move(int dx, int dy = 0);
};

// Free functions (Task 1.10)
Rect bounding_rect(Rect r1, Rect r2);  // smallest rect containing both r1 and r2
void print_rect(Rect& r);

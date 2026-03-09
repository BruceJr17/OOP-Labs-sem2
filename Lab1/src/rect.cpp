#include "rect.h"
#include <algorithm>
using namespace std;
void Rect::set_normalised(int left, int right, int top, int bottom) {
    x      = std::min(left, right);
    y      = std::min(top,  bottom);
    width  = std::max(left, right) - x;
    height = std::max(top,  bottom) - y;
}
// ------------------------------------------------------------------ constructors

// Default constructor: initialise all fields to zero
Rect::Rect() : x(0), y(0), width(0), height(0) {
    cout << "[Rect] default ctor, this=" << this << "\n";
}

// Parameterized constructor
// Task 5: previously stored {left, right, top, bottom} directly.
//         Now we convert to {x, y, width, height}.
// Also handles the case where the caller accidentally swaps left/right or top/bottom.

Rect::Rect(int left, int right, int top, int bottom) {
    set_normalised(left, right, top, bottom);
    cout << "[Rect] ctor(4 params), this=" << this << "\n";
}

// Copy constructor (Task 1.6): copies all fields one-by-one
// (safe here because Rect owns no heap memory)

Rect::Rect(const Rect& other)
    : x(other.x), y(other.y), width(other.width), height(other.height) {
    cout << "[Rect] copy ctor, this=" << this << "\n";
}

// Destructor (Task 1.5): nothing to free for Rect, but we log the call
Rect::~Rect() {
    cout << "[Rect] dtor, this=" << this << "\n";
}

// ------------------------------------------------------------------ getters
// Task 5: the stored fields changed, but the public interface stayed the same.
//         Callers never need to know about the internal representation.

int Rect::get_left()   const { return x; }
int Rect::get_right()  const { return x + width; }
int Rect::get_bottom() const { return y; }
int Rect::get_top()    const { return y + height; }

int Rect::get_width()  const { return width; }
int Rect::get_height() const { return height; }
int Rect::get_square() const { return width * height; }

// ------------------------------------------------------------------ setters

// set_width preserves the left edge (x), only extends to the right
void Rect::set_width(int w) {
    width = w;
}

// set_height preserves the bottom edge (y), only extends upward
void Rect::set_height(int h) {
    height = h;
}

void Rect::set_all(int left, int right, int top, int bottom) {
    set_normalised(left, right, top, bottom);
}

// ------------------------------------------------------------------ inflate (Task 1.9)

// Push all four edges outward by the same amount
void Rect::inflate(int amount) {
    x -= amount;
    y -= amount;
    width += 2 * amount;
    height += 2 * amount;
}

// Push left/right edges by dw, top/bottom edges by dh
void Rect::inflate(int dw, int dh) {
    x -= dw;
    y -= dh;
    width += 2 * dw;
    height += 2 * dh;
}

// Push each edge by its own amount
void Rect::inflate(int d_left, int d_right, int d_top, int d_bottom) {
    x -= d_left;
    width += d_left + d_right;
    y -= d_bottom;
    height += d_top + d_bottom;
}

// ------------------------------------------------------------------ move (Task 1.9)

void Rect::move(int dx, int dy) {
    x += dx;
    y += dy;
}

// ------------------------------------------------------------------ free functions (Task 1.10)

// When r1 and r2 are passed by value, the copy constructor is called for each.
Rect bounding_rect(Rect r1, Rect r2) {
    int left = std::min(r1.get_left(), r2.get_left());
    int right = std::max(r1.get_right(), r2.get_right());
    int bottom = std::min(r1.get_bottom(), r2.get_bottom());
    int top = std::max(r1.get_top(), r2.get_top());
    return Rect(left, right, top, bottom);
}

// Takes r by reference - no copy constructor is called
void print_rect(Rect& r) {
    cout << "Rect: left=" << r.get_left()
        << " right=" << r.get_right()
        << " top=" << r.get_top()
        << " bottom=" << r.get_bottom()
        << " w=" << r.get_width()
        << " h=" << r.get_height()
        << "\n";
}

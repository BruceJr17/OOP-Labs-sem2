#include <cassert>
#include "../src/rect.h"

int main() {
    Rect r(0, 10, 0, 5);
    assert(r.get_width()  == 10);
    assert(r.get_height() == 5);
    assert(r.get_square() == 50);

    r.set_width(20);
    assert(r.get_left()  == 0);
    assert(r.get_right() == 20);
    assert(r.get_width() == 20);

    r.set_height(15);
    assert(r.get_bottom() == 0);
    assert(r.get_top()    == 15);
    assert(r.get_height() == 15);

    assert(r.get_square() == 300);

    return 0;
}

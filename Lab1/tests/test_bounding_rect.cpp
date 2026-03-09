#include <cassert>
#include "../src/rect.h"

int main() {
    Rect r1(0, 5, 0, 5);
    Rect r2(3, 8, 2, 7);
    Rect b = bounding_rect(r1, r2);
    assert(b.get_left()   == 0);
    assert(b.get_right()  == 8);
    assert(b.get_bottom() == 0);
    assert(b.get_top()    == 7);

    Rect r3(0, 10, 0, 10);
    Rect r4(2,  8, 2,  8);
    Rect b2 = bounding_rect(r3, r4);
    assert(b2.get_left()   == 0);
    assert(b2.get_right()  == 10);
    assert(b2.get_bottom() == 0);
    assert(b2.get_top()    == 10);

    Rect r5(5, 0, 5, 0);
    assert(r5.get_left()   == 0);
    assert(r5.get_right()  == 5);
    assert(r5.get_bottom() == 0);
    assert(r5.get_top()    == 5);

    return 0;
}

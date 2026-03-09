#include <cassert>
#include "../src/rect.h"

int main() {
    Rect r0;
    assert(r0.get_left()   == 0);
    assert(r0.get_right()  == 0);
    assert(r0.get_top()    == 0);
    assert(r0.get_bottom() == 0);

    Rect r1(1, 5, 2, 8);
    assert(r1.get_left()   == 1);
    assert(r1.get_right()  == 5);
    assert(r1.get_bottom() == 2);
    assert(r1.get_top()    == 8);

    Rect r2(r1);
    assert(r2.get_left()   == r1.get_left());
    assert(r2.get_right()  == r1.get_right());

    r0.set_all(3, 7, 1, 5);
    assert(r0.get_left()   == 3);
    assert(r0.get_right()  == 7);
    assert(r0.get_bottom() == 1);
    assert(r0.get_top()    == 5);

    return 0;
}

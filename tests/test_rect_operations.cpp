#include <cassert>
#include "../src/rect.h"

int main() {
    Rect r(0, 10, 0, 10);
    r.move(5, 3);
    assert(r.get_left()   == 5);
    assert(r.get_right()  == 15);
    assert(r.get_bottom() == 3);
    assert(r.get_top()    == 13);

    r.move(2);
    assert(r.get_left()   == 7);
    assert(r.get_right()  == 17);
    assert(r.get_bottom() == 3);

    Rect r2(0, 10, 0, 10);
    r2.inflate(2);
    assert(r2.get_left()   == -2);
    assert(r2.get_right()  == 12);
    assert(r2.get_bottom() == -2);
    assert(r2.get_top()    == 12);

    Rect r3(0, 10, 0, 10);
    r3.inflate(1, 3);
    assert(r3.get_left()   == -1);
    assert(r3.get_right()  == 11);
    assert(r3.get_bottom() == -3);
    assert(r3.get_top()    == 13);

    Rect r4(0, 10, 0, 10);
    r4.inflate(1, 2, 3, 4);
    assert(r4.get_left()   == -1);
    assert(r4.get_right()  == 12);
    assert(r4.get_top()    == 13);
    assert(r4.get_bottom() == -4);

    return 0;
}

#include "my_string.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
using namespace std;
// ------------------------------------------------------------------ helper

void MyString::free_buf() {
    delete[] buf;
    buf = nullptr;
    len = 0;
}

// ------------------------------------------------------------------ constructors

// Default constructor: allocate a single-byte buffer holding just '\0'

MyString::MyString() : buf(nullptr), len(0) {
    // no allocation for empty string
}

MyString::MyString(const char* str) : buf(nullptr), len(0) {
    len = strlen(str);
    buf = new char[len + 1];
    memcpy(buf, str, len + 1);
}

// Copy constructor: MUST allocate new memory and copy the data.
// Copying only the pointer (shallow copy) would mean two objects share the
// same buffer - the second destructor would then free already-freed memory.

MyString::MyString(const MyString& other) : buf(nullptr), len(other.len) {
    if (other.buf) {
       buf = new char[len + 1];
       memcpy(buf, other.buf, len + 1);
  }
}

// ------------------------------------------------------------------ destructor

MyString::~MyString() {
    free_buf();
}

// ------------------------------------------------------------------ element access

char MyString::get(int i) const { return buf ? buf[i] : '\0'; }
void MyString::set(int i, char c) { if (buf) buf[i] = c; }

int MyString::get_len() const { return len; }

// ------------------------------------------------------------------ other methods

// Replace current contents - free old buffer, then allocate a fresh one

void MyString::set_new_string(const char* str) {
    free_buf();
    len = strlen(str);
    buf = new char[len + 1];
    memcpy(buf, str, len + 1);
}

void MyString::print() const {
    if (buf) cout << buf;
}

// Read a line of unknown length from stdin.
// Start with a small buffer and double it whenever it is about to overflow.

void MyString::read_line() {
    free_buf();

    int capacity = 16;
    buf = new char[capacity];
    len = 0;

    char c;
    while (cin.get(c) && c != '\n') {
        if (len + 1 >= capacity) {
            capacity *= 2;
            char* new_buf = new char[capacity];
            memcpy(new_buf, buf, len);
            delete[] buf;
            buf = new_buf;
        }
        buf[len++] = c;
    }
    buf[len] = '\0';
}

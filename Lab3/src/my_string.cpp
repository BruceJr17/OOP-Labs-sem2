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
// ── copy assignment operator ──────────────────────────────────────────────────
// Task 1.1: works just like copy constructor but must handle self-assignment
MyString& MyString::operator=(const MyString& other) {
    if (this == &other) return *this;  // self-assignment guard
    free_buf();
    len = other.len;
    buf = new char[len + 1];
    memcpy(buf, other.buf, len + 1);
    return *this;
}

// assign from c-string
MyString& MyString::operator=(const char* str) {
    free_buf();
    if (!str) { buf = new char[1]; buf[0] = '\0'; len = 0; return *this; }
    len = (int)strlen(str);
    buf = new char[len + 1];
    memcpy(buf, str, len + 1);
    return *this;
}

// ── move constructor ──────────────────────────────────────────────────────────
// Task 1.3: "steal" the buffer from the other object instead of copying
MyString::MyString(MyString&& other) {
    buf       = other.buf;  // take their pointer
    len       = other.len;
    other.buf = new char[1];  // leave other in valid empty state
    other.buf[0] = '\0';
    other.len = 0;
}

// ── move assignment operator ──────────────────────────────────────────────────
MyString& MyString::operator=(MyString&& other) {
    if (this == &other) return *this;
    free_buf();
    buf       = other.buf;
    len       = other.len;
    other.buf = new char[1];
    other.buf[0] = '\0';
    other.len = 0;
    return *this;
}

// ── concatenation operators ───────────────────────────────────────────────────
// Task 1.2.4: += appends to this string
MyString& MyString::operator+=(const MyString& other) {
    int new_len = len + other.len;
    char* new_buf = new char[new_len + 1];
    memcpy(new_buf,       buf,       len);
    memcpy(new_buf + len, other.buf, other.len + 1);
    free_buf();
    buf = new_buf;
    len = new_len;
    return *this;
}

MyString& MyString::operator+=(const char* str) {
    if (!str) return *this;
    int slen    = (int)strlen(str);
    int new_len = len + slen;
    char* new_buf = new char[new_len + 1];
    memcpy(new_buf,       buf, len);
    memcpy(new_buf + len, str, slen + 1);
    free_buf();
    buf = new_buf;
    len = new_len;
    return *this;
}

// + returns a new string (does not modify this)
MyString MyString::operator+(const MyString& other) const {
    MyString result(*this);  // copy of this
    result += other;
    return result;
}

MyString MyString::operator+(const char* str) const {
    MyString result(*this);
    result += str;
    return result;
}

// global: "literal" + MyString
MyString operator+(const char* lhs, const MyString& rhs) {
    MyString result(lhs);
    result += rhs;
    return result;
}

// ── comparison operators ──────────────────────────────────────────────────────
// Task 1.4: lexicographic comparison using strcmp
bool MyString::operator==(const MyString& other) const {
    return strcmp(buf ? buf : "", other.buf ? other.buf : "") == 0;
}
bool MyString::operator==(const char* str) const {
    return strcmp(buf ? buf : "", str ? str : "") == 0;
}
bool MyString::operator!=(const MyString& other) const {
    return !(*this == other);
}
bool MyString::operator!=(const char* str) const {
    return !(*this == str);
}
bool MyString::operator<(const MyString& other) const {
    return strcmp(buf ? buf : "", other.buf ? other.buf : "") < 0;
}
bool MyString::operator<(const char* str) const {
    return strcmp(buf ? buf :"", str ? str : "") < 0;
}
bool MyString::operator<=(const MyString& other) const {
    return strcmp(buf ? buf : "", other.buf ? other.buf : "") <= 0;
}
bool MyString::operator<=(const char* str) const {
    return strcmp(buf ? buf : "", str ? str : "") <= 0;
}

// global: "literal" == MyString  and  "literal" < MyString  etc.
// These are needed because the left operand is const char*, not MyString
bool operator==(const char* lhs, const MyString& rhs) { return rhs == lhs; }
bool operator!=(const char* lhs, const MyString& rhs) { return rhs != lhs; }
bool operator< (const char* lhs, const MyString& rhs) { return rhs >  lhs; }
bool operator<=(const char* lhs, const MyString& rhs) { return rhs >= lhs; }

// ── index operator ────────────────────────────────────────────────────────────
// Task 1.5: returns a reference so caller can do s[0] = 'X'
char& MyString::operator[](int i) {
    if (i < 0 || i >= len)
        throw std::out_of_range("MyString index out of range");
    return buf[i];
}
char MyString::operator[](int i) const {
    if (i < 0 || i >= len)
        throw std::out_of_range("MyString index out of range");
    return buf[i];
}

// ── stream operators ──────────────────────────────────────────────────────────
// Task 1.6: << prints the string, >> reads a line
std::ostream& operator<<(std::ostream& os, const MyString& s) {
    if (s.buf) os << s.buf;
    return os;
}

std::istream& operator>>(std::istream& is, MyString& s) {
    s.read_line();
    return is;
}
bool MyString::operator>(const MyString& other)  const { return other < *this; }
bool MyString::operator>=(const MyString& other) const { return other <= *this; }

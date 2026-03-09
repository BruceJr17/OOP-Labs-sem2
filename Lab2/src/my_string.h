#pragma once
#include <cstring>

// Task 2.3 - MyString: a class that owns a heap-allocated C string.
//
// This demonstrates the "resource owner" pattern:
//   - the object is solely responsible for the memory it holds;
//   - memory is allocated in the constructors;
//   - memory is freed in the destructor (and when replaced via set_new_string);
//   - the copy constructor performs a deep copy so each object has its own buffer.

class MyString {
private:
    char* buf;  // heap-allocated null-terminated string
    int   len;  // length, NOT counting the null terminator

    void free_buf();  // internal helper: free buf and reset len

public:
    MyString();                       // creates an empty string
    MyString(const char* str);        // copies str into newly allocated memory
    MyString(const MyString& other);  // deep copy
    ~MyString();

    char get(int i) const;    // return the i-th character
    void set(int i, char c);  // overwrite the i-th character

    int get_len() const;

    void set_new_string(const char* str);  // replace contents (frees old memory first)
    void print() const;                    // print to stdout (no newline)
    void read_line();                      // read a line from stdin (any length)
};
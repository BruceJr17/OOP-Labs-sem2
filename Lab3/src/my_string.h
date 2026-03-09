#pragma once
#include <cstring>
#include <iostream>

class MyString {

private:
    char* buf;
    int   len;
    void free_buf();

public:
    // ── constructors / destructor ─────────────────────────────────────────
    MyString();
    MyString(const char* str);
    MyString(const MyString& other);   // copy constructor
    MyString(MyString&& other);        // move constructor  (Task 1.3)
    ~MyString();

    // ── assignment operators ──────────────────────────────────────────────
    MyString& operator=(const MyString& other);  // copy assign  (Task 1.1)
    MyString& operator=(const char* str);        // assign from c-string
    MyString& operator=(MyString&& other);       // move assign  (Task 1.3)

    // ── concatenation operators ───────────────────────────────────────────
    MyString& operator+=(const MyString& other); // Task 1.2.4
    MyString& operator+=(const char* str);
    MyString  operator+(const MyString& other) const;
    MyString  operator+(const char* str)       const;

    // ── comparison operators ──────────────────────────────────────────────
    bool operator==(const MyString& other) const; // Task 1.4
    bool operator==(const char* str)       const;
    bool operator!=(const MyString& other) const;
    bool operator!=(const char* str)       const;
    bool operator< (const MyString& other) const;
    bool operator< (const char* str)       const;
    bool operator<=(const MyString& other) const;
    bool operator<=(const char* str)       const;
    bool operator> (const MyString& other) const;
    bool operator>=(const MyString& other) const;

    // ── index operator ────────────────────────────────────────────────────
    char& operator[](int i);             // Task 1.5
    char  operator[](int i) const;

    // ── stream operators (global, declared friend) ────────────────────────
    friend std::ostream& operator<<(std::ostream& os, const MyString& s);
    friend std::istream& operator>>(std::istream& is, MyString& s);

    // ── global concat: "literal" + MyString ──────────────────────────────
    friend MyString operator+(const char* lhs, const MyString& rhs);

    // ── existing methods ──────────────────────────────────────────────────
    char get(int i)    const;
    void set(int i, char c);
    int  get_len()     const;
    void set_new_string(const char* str);
    void print()       const;
    void read_line();
};

    // global comparison operators: "literal" == mystring etc.
    bool operator==(const char* lhs, const MyString& rhs);
    bool operator!=(const char* lhs, const MyString& rhs);
    bool operator< (const char* lhs, const MyString& rhs);
    bool operator<=(const char* lhs, const MyString& rhs);

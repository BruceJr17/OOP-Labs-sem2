#pragma once
#include "my_string.h"

// Task 2.4 - TextWrapper: the "algorithm object" pattern.
//
// A large algorithm is split into small, focused methods.
// Variables that would otherwise be shared across many functions are stored
// as fields, making the code easy to read and maintain.

class TextWrapper {
private:
    const MyString& input;   // the text to be wrapped (reference - no copy)
    int line_width;          // maximum characters per output line

    // Algorithm state - these fields replace what would be local variables
    // scattered across multiple standalone functions

    int  pos;             // current read position in input
    int  line_pos;        // number of characters already written on the current line
    char word_buf[512];   // characters of the word currently being accumulated
    int  word_len;        // number of characters in word_buf

    // Small, single-responsibility helper methods

    bool is_end()   const;   // true when we have consumed all of input
    bool is_space() const;   // true when the current character is whitespace
    void flush_word();       // output the accumulated word, wrapping if necessary
    void next_line();        // emit a newline and reset line_pos

public:
    TextWrapper(const MyString& str, int width);
    void print_wrapped();  // run the algorithm and print wrapped text to stdout
};
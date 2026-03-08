#include "text_wrapper.h"
#include <iostream>
using namespace std;

TextWrapper::TextWrapper(const MyString& str, int width)
    : input(str), line_width(width), pos(0), line_pos(0), word_len(0) {
}

bool TextWrapper::is_end() const {
    return pos >= input.get_len();
}

bool TextWrapper::is_space() const {
    char c = input.get(pos);
    return c == ' ' || c == '\t';
}

void TextWrapper::next_line() {
    cout << '\n';
    line_pos = 0;
}

// Output the accumulated word.
// If it doesn't fit on the current line, start a new line first.
// Handles words longer than line_width by splitting them across lines.
void TextWrapper::flush_word() {
    if (word_len == 0) return;

    // word doesn't fit on the current line - wrap
    if (line_pos > 0 && line_pos + word_len > line_width) {
        next_line();
    }

    for (int i = 0; i < word_len; i++) {
        cout << word_buf[i];
        line_pos++;
        // line is full mid-word (extremely long word) - force a wrap
        if (line_pos >= line_width && i < word_len - 1) {
            next_line();
        }
    }

    word_len = 0;
}

void TextWrapper::print_wrapped() {
    pos = 0;
    line_pos = 0;
    word_len = 0;

    while (!is_end()) {
        if (is_space()) {
            // hit a space: flush whatever word we have accumulated
            flush_word();

            // print the space separator only if we are not at the start of a line
            if (line_pos > 0 && line_pos < line_width) {
                cout << ' ';
                line_pos++;
            }
            pos++;
        }
        else {
            // accumulate characters into the word buffer
            if (word_len < 511)
                word_buf[word_len++] = input.get(pos);
            pos++;
        }
    }

    // flush any remaining word at end of input
    flush_word();
    cout << '\n';
}
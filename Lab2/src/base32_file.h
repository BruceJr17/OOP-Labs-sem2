#pragma once
#include "base_file.h"

class Base32File : public BaseFile {
private:
    // encoding table — default "A..Z1..6"
    char table[33];

    // leftover bits between sequential write calls
    int  write_buffer;
    int  write_bits;

    // leftover bits between sequential read calls
    int  read_buffer;
    int  read_bits;

public:
    Base32File();
    Base32File(const char *path, const char *mode,
               const char *custom_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456");
    Base32File(FILE *f,
               const char *custom_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456");
    ~Base32File();

    // override read/write with Base32 encoding/decoding
    size_t write(const void *buf, size_t n_bytes) override;
    size_t read(void  *buf, size_t max_bytes)     override;

    // flush any leftover bits to file
    void flush_write();
};

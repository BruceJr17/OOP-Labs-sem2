#pragma once
#include "ifile.h"
#include "base32_file.h"
#include "rle_file.h"
#include <cstring>

// ── Base32File2: inherits IFile, uses another IFile for actual I/O ────────────
class Base32File2 : public IFile {
private:
    IFile *inner;  // owned — we delete it in destructor

    // same stateful encode/decode fields as Base32File
    char table[33];
    int  write_buffer, write_bits;
    int  read_buffer,  read_bits;

    void flush_write();

public:
    Base32File2(IFile *inner_file,
                const char *custom_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456");
    ~Base32File2();

    bool   can_read()  const override { return inner->can_read();  }
    bool   can_write() const override { return inner->can_write(); }
    size_t write(const void *buf, size_t n_bytes) override;
    size_t read(void  *buf, size_t max_bytes)     override;
};

// ── RleFile2: inherits IFile, uses another IFile for actual I/O ───────────────
class RleFile2 : public IFile {
private:
    IFile *inner;  // owned

    // same stateful RLE fields as RleFile
    unsigned char write_last_byte;
    int           write_count;
    bool          write_started;
    unsigned char read_current_byte;
    int           read_remaining;

    void flush_write();

public:
    RleFile2(IFile *inner_file);
    ~RleFile2();

    bool   can_read()  const override { return inner->can_read();  }
    bool   can_write() const override { return inner->can_write(); }
    size_t write(const void *buf, size_t n_bytes) override;
    size_t read(void  *buf, size_t max_bytes)     override;
};

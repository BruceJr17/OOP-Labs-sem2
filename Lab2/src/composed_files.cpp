#include "composed_files.h"
#include <cstring>
#include <iostream>
using namespace std;

//
// Base32File2
//

Base32File2::Base32File2(IFile *inner_file, const char *custom_table)
    : inner(inner_file),
      write_buffer(0), write_bits(0),
      read_buffer(0),  read_bits(0)
{
    strncpy(table, custom_table, 32);
    table[32] = '\0';
    cout << "[Base32File2] ctor, this=" << this << "\n";
}

Base32File2::~Base32File2() {
    flush_write();
    cout << "[Base32File2] dtor, this=" << this << "\n";
    delete inner;
}

void Base32File2::flush_write() {
    if (write_bits > 0) {
        int chunk = (write_buffer << (5 - write_bits)) & 0x1F;
        char c = table[chunk];
        inner->write(&c, 1);
        write_buffer = 0;
        write_bits   = 0;
    }
}

size_t Base32File2::write(const void *buf, size_t n_bytes) {
    const unsigned char *src = (const unsigned char *)buf;
    size_t written = 0;
    for (size_t i = 0; i < n_bytes; i++) {
        write_buffer = (write_buffer << 8) | src[i];
        write_bits  += 8;
        while (write_bits >= 5) {
            write_bits -= 5;
            int  chunk = (write_buffer >> write_bits) & 0x1F;
            char c     = table[chunk];
            if (inner->write(&c, 1) == 1) written++;
        }
    }
    return written;
}

size_t Base32File2::read(void *buf, size_t max_bytes) {
    unsigned char *dst = (unsigned char *)buf;
    size_t produced = 0;
    while (produced < max_bytes) {
        if (read_bits >= 8) {
            read_bits -= 8;
            dst[produced++] = (read_buffer >> read_bits) & 0xFF;
            continue;
        }
        char c;
        if (inner->read(&c, 1) != 1) break;
        const char *pos = strchr(table, c);
        if (!pos) break;
        read_buffer = (read_buffer << 5) | (int)(pos - table);
        read_bits  += 5;
    }
    return produced;
}


// RleFile2


RleFile2::RleFile2(IFile *inner_file)
    : inner(inner_file),
      write_last_byte(0), write_count(0), write_started(false),
      read_current_byte(0), read_remaining(0)
{
    cout << "[RleFile2] ctor, this=" << this << "\n";
}

RleFile2::~RleFile2() {
    flush_write();
    cout << "[RleFile2] dtor, this=" << this << "\n";
    delete inner;
}

void RleFile2::flush_write() {
    if (!write_started) return;
    while (write_count > 0) {
        int           chunk = write_count > 255 ? 255 : write_count;
        unsigned char cnt   = (unsigned char)chunk;
        inner->write(&cnt,             1);
        inner->write(&write_last_byte, 1);
        write_count -= chunk;
    }
    write_started = false;
}

size_t RleFile2::write(const void *buf, size_t n_bytes) {
    const unsigned char *src = (const unsigned char *)buf;
    size_t consumed = 0;
    for (size_t i = 0; i < n_bytes; i++) {
        unsigned char byte = src[i];
        if (!write_started) {
            write_last_byte = byte;
            write_count     = 1;
            write_started   = true;
        } else if (byte == write_last_byte && write_count < 255) {
            write_count++;
        } else {
            flush_write();
            write_last_byte = byte;
            write_count     = 1;
            write_started   = true;
        }
        consumed++;
    }
    return consumed;
}

size_t RleFile2::read(void *buf, size_t max_bytes) {
    unsigned char *dst = (unsigned char *)buf;
    size_t produced = 0;
    while (produced < max_bytes) {
        if (read_remaining > 0) {
            dst[produced++] = read_current_byte;
            read_remaining--;
            continue;
        }
        unsigned char cnt, byte;
        if (inner->read(&cnt,  1) != 1) break;
        if (inner->read(&byte, 1) != 1) break;
        read_current_byte = byte;
        read_remaining    = cnt - 1;
        dst[produced++]   = byte;
    }
    return produced;
}

#include "base32_file.h"
#include <cstring>
#include <iostream>
using namespace std;

// ── helpers ──────────────────────────────────────────────────────────────────

static void init_table(char *dst, const char *src) {
    strncpy(dst, src, 32);
    dst[32] = '\0';
}

// ── constructors / destructor ─────────────────────────────────────────────────

Base32File::Base32File()
    : BaseFile(), write_buffer(0), write_bits(0),
      read_buffer(0), read_bits(0)
{
    init_table(table, "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456");
    cout << "[Base32File] default ctor, this=" << this << "\n";
}

Base32File::Base32File(const char *path, const char *mode,
                       const char *custom_table)
    : BaseFile(path, mode), write_buffer(0), write_bits(0),
      read_buffer(0), read_bits(0)
{
    init_table(table, custom_table);
    cout << "[Base32File] path ctor, this=" << this << "\n";
}

Base32File::Base32File(FILE *f, const char *custom_table)
    : BaseFile(f), write_buffer(0), write_bits(0),
      read_buffer(0), read_bits(0)
{
    init_table(table, custom_table);
    cout << "[Base32File] FILE* ctor, this=" << this << "\n";
}

Base32File::~Base32File() {
    flush_write();
    cout << "[Base32File] dtor, this=" << this << "\n";
}

// ── flush leftover bits ───────────────────────────────────────────────────────

void Base32File::flush_write() {
    if (write_bits > 0) {
        // pad remaining bits to 5
        int chunk = (write_buffer << (5 - write_bits)) & 0x1F;
        char c = table[chunk];
        write_raw(&c, 1);
        write_buffer = 0;
        write_bits   = 0;
    }
}

// ── write: encode raw bytes as Base32 ────────────────────────────────────────

size_t Base32File::write(const void *buf, size_t n_bytes) {
    const unsigned char *src = (const unsigned char *)buf;
    size_t written = 0;

    for (size_t i = 0; i < n_bytes; i++) {
        // push 8 bits into the buffer
        write_buffer = (write_buffer << 8) | src[i];
        write_bits  += 8;

        // emit as many 5-bit chunks as possible
        while (write_bits >= 5) {
            write_bits -= 5;
            int chunk = (write_buffer >> write_bits) & 0x1F;
            char c = table[chunk];
            if (write_raw(&c, 1) == 1)
                written++;
        }
    }
    return written;
}

// ── read: decode Base32 back to raw bytes ─────────────────────────────────────

size_t Base32File::read(void *buf, size_t max_bytes) {
    unsigned char *dst = (unsigned char *)buf;
    size_t produced = 0;

    while (produced < max_bytes) {
        // if we have a full byte ready, emit it
        if (read_bits >= 8) {
            read_bits -= 8;
            dst[produced++] = (read_buffer >> read_bits) & 0xFF;
            continue;
        }

        // read one Base32 character from the file
        char c;
        if (read_raw(&c, 1) != 1) break; // EOF

        // find its index in the table
        const char *pos = strchr(table, c);
        if (!pos) break; // invalid character
        int chunk = (int)(pos - table);

        read_buffer = (read_buffer << 5) | chunk;
        read_bits  += 5;
    }
    return produced;
}

#include "rle_file.h"
#include <iostream>
using namespace std;

// ── constructors / destructor ─────────────────────────────────────────────────

RleFile::RleFile()
    : BaseFile(),
      write_last_byte(0), write_count(0), write_started(false),
      read_current_byte(0), read_remaining(0)
{
    cout << "[RleFile] default ctor, this=" << this << "\n";
}

RleFile::RleFile(const char *path, const char *mode)
    : BaseFile(path, mode),
      write_last_byte(0), write_count(0), write_started(false),
      read_current_byte(0), read_remaining(0)
{
    cout << "[RleFile] path ctor, this=" << this << "\n";
}

RleFile::RleFile(FILE *f)
    : BaseFile(f),
      write_last_byte(0), write_count(0), write_started(false),
      read_current_byte(0), read_remaining(0)
{
    cout << "[RleFile] FILE* ctor, this=" << this << "\n";
}

RleFile::~RleFile() {
    flush_write();
    cout << "[RleFile] dtor, this=" << this << "\n";
}

// ── flush: write the last pending run to the file ─────────────────────────────
// RLE format: [count][byte] where count is 1 byte (1..255)

void RleFile::flush_write() {
    if (!write_started) return;

    while (write_count > 0) {
        // max 255 per RLE chunk
        int chunk = write_count > 255 ? 255 : write_count;
        unsigned char cnt = (unsigned char)chunk;
        write_raw(&cnt,             1);
        write_raw(&write_last_byte, 1);
        write_count -= chunk;
    }
    write_started = false;
}

// ── write: compress with RLE ──────────────────────────────────────────────────

size_t RleFile::write(const void *buf, size_t n_bytes) {
    const unsigned char *src = (const unsigned char *)buf;
    size_t consumed = 0;

    for (size_t i = 0; i < n_bytes; i++) {
        unsigned char byte = src[i];

        if (!write_started) {
            // first byte ever
            write_last_byte = byte;
            write_count     = 1;
            write_started   = true;
        } else if (byte == write_last_byte && write_count < 255) {
            // same byte — extend the run
            write_count++;
        } else {
            // different byte — flush the previous run, start a new one
            flush_write();
            write_last_byte = byte;
            write_count     = 1;
            write_started   = true;
        }
        consumed++;
    }
    return consumed;
}

// ── read: decompress RLE ──────────────────────────────────────────────────────

size_t RleFile::read(void *buf, size_t max_bytes) {
    unsigned char *dst = (unsigned char *)buf;
    size_t produced = 0;

    while (produced < max_bytes) {
        // if we still have bytes left in the current run, emit them
        if (read_remaining > 0) {
            dst[produced++] = read_current_byte;
            read_remaining--;
            continue;
        }

        // read next [count][byte] pair from file
        unsigned char cnt, byte;
        if (read_raw(&cnt,  1) != 1) break; // EOF
        if (read_raw(&byte, 1) != 1) break;

        read_current_byte = byte;
        read_remaining    = cnt - 1; // emit first one now
        dst[produced++]   = byte;
    }
    return produced;
}

#include "base_file.h"
#include <iostream>
using namespace std;

// default constructor — no file open
BaseFile::BaseFile() : file(nullptr) {
    cout << "[BaseFile] default ctor, this=" << this << "\n";
}

// open file by path
BaseFile::BaseFile(const char *path, const char *mode) {
    file = fopen(path, mode);
    cout << "[BaseFile] path ctor, this=" << this << "\n";
}

// take ownership of existing FILE*
BaseFile::BaseFile(FILE *f) : file(f) {
    cout << "[BaseFile] FILE* ctor, this=" << this << "\n";
}

// destructor closes file if open
BaseFile::~BaseFile() {
    cout << "[BaseFile] dtor, this=" << this << "\n";
    if (file) {
        fclose(file);
        file = nullptr;
    }
}

bool BaseFile::is_open()   const { return file != nullptr; }
bool BaseFile::can_read()  const { return file != nullptr; }
bool BaseFile::can_write() const { return file != nullptr; }

size_t BaseFile::write_raw(const void *buf, size_t n_bytes) {
    if (!file) return 0;
    return fwrite(buf, 1, n_bytes, file);
}

size_t BaseFile::read_raw(void *buf, size_t max_bytes) {
    if (!file) return 0;
    return fread(buf, 1, max_bytes, file);
}

// base read/write just call raw versions
size_t BaseFile::write(const void *buf, size_t n_bytes) {
    return write_raw(buf, n_bytes);
}

size_t BaseFile::read(void *buf, size_t max_bytes) {
    return read_raw(buf, max_bytes);
}

long BaseFile::tell() const {
    if (!file) return -1;
    return ftell(file);
}

bool BaseFile::seek(long offset) {
    if (!file) return false;
    return fseek(file, offset, SEEK_SET) == 0;
}

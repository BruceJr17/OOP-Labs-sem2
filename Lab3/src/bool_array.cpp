#include "bool_array.h"
#include <cstring>

BoolArray::BoolArray(int size, bool fill) : bits(size) {
    int nb = bytes_needed(size);
    data = new unsigned char[nb];
    memset(data, fill ? 0xFF : 0x00, nb);
}

BoolArray::BoolArray(const BoolArray& other) : bits(other.bits) {
    int nb = bytes_needed(bits);
    data = new unsigned char[nb];
    memcpy(data, other.data, nb);
}

BoolArray::~BoolArray() {
    delete[] data;
}

BoolArray& BoolArray::operator=(const BoolArray& other) {
    if (this == &other) return *this;
    delete[] data;
    bits = other.bits;
    int nb = bytes_needed(bits);
    data = new unsigned char[nb];
    memcpy(data, other.data, nb);
    return *this;
}

BoolArray::BitProxy BoolArray::operator[](int i) {
    if (i < 0 || i >= bits)
        throw std::out_of_range("BoolArray index out of range");
    return BitProxy(data, i);
}

bool BoolArray::operator[](int i) const {
    if (i < 0 || i >= bits)
        throw std::out_of_range("BoolArray index out of range");
    return (data[i / 8] >> (i % 8)) & 1;
}

void BoolArray::resize(int new_size, bool fill) {
    int old_bytes = bytes_needed(bits);
    int new_bytes = bytes_needed(new_size);
    unsigned char* new_data = new unsigned char[new_bytes];

    // copy existing bytes
    int copy_bytes = new_bytes < old_bytes ? new_bytes : old_bytes;
    memcpy(new_data, data, copy_bytes);

    // fill new bytes if growing
    if (new_bytes > old_bytes)
        memset(new_data + old_bytes, fill ? 0xFF : 0x00, new_bytes - old_bytes);

    // fill individual bits from old bit-count to new_size
    // (handles the case where new bits fall in an already-existing byte)
    delete[] data;
    data = new_data;
    int old_bits = bits;
    bits = new_size;

    if (new_size > old_bits) {
        for (int i = old_bits; i < new_size; i++) {
            int  byte = i / 8;
            int  bit  = i % 8;
            if (fill) new_data[byte] |=  (1 << bit);
            else      new_data[byte] &= ~(1 << bit);
        }
    }
}


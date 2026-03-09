#pragma once
#include <stdexcept>

// BoolArray: a dynamic array of bools packed into bytes.
// Each bool uses exactly 1 bit, so n bools use ceil(n/8) bytes.
//
// The key trick: to access bit i, we look at byte[i/8] and
// check/set bit number (i%8) within that byte.

class BoolArray {
private:
    unsigned char* data;  // heap-allocated bytes
    int            bits;  // number of logical bools in the array

    // how many bytes do we need to store 'n' bits?
    static int bytes_needed(int n) { return (n + 7) / 8; }

    // Proxy class: returned by operator[] to allow both reading and writing.
    // When you write  ar[i] = true  the proxy's operator= is called.
    // When you read   bool x = ar[i]  the implicit conversion is called.
    struct BitProxy {
        unsigned char* data;
        int            index;

        BitProxy(unsigned char* d, int i) : data(d), index(i) {}

        // write: ar[i] = value
        BitProxy& operator=(bool value) {
            int  byte = index / 8;
            int  bit  = index % 8;
            if (value) data[byte] |=  (1 << bit);  // set bit
            else       data[byte] &= ~(1 << bit);  // clear bit
            return *this;
        }

        // chain assignment: ar[i] = ar[j] = true
        BitProxy& operator=(const BitProxy& other) {
            return *this = (bool)other;
        }

        // read: implicit conversion to bool
        operator bool() const {
            int byte = index / 8;
            int bit  = index % 8;
            return (data[byte] >> bit) & 1;
        }

        // logical operators so  !ar[i]  and  ar[i] && ar[j]  work
        bool operator!()              const { return !(bool)*this; }
        bool operator&&(bool other)   const { return (bool)*this && other; }
        bool operator||(bool other)   const { return (bool)*this || other; }
        bool operator!=(bool other)   const { return (bool)*this != other; }
    };

public:
    BoolArray(int size, bool fill = false);
    BoolArray(const BoolArray& other);   // deep copy
    ~BoolArray();

    BoolArray& operator=(const BoolArray& other);

    BitProxy operator[](int i);
    bool     operator[](int i) const;

    int  size()   const { return bits; }
    void resize(int new_size, bool fill = false);
};

#include <cassert>
#include <cstdlib>
#include <cstring>
#include "../src/base32_file.h"

int main() {
    const int DATA_SIZE = 41000;
    const int BUF_SIZE  = 256;

    unsigned char *original = new unsigned char[DATA_SIZE];
    for (int i = 0; i < DATA_SIZE; i++)
        original[i] = (unsigned char)(rand() % 256);

    // write encoded
    {
        Base32File bf("build/tst_b32.bin", "wb");
        assert(bf.is_open());
        int written = 0;
        while (written < DATA_SIZE) {
            int chunk = (DATA_SIZE - written) < BUF_SIZE
                      ? (DATA_SIZE - written) : BUF_SIZE;
            size_t n = bf.write(original + written, chunk);
            assert(n > 0);
            written += chunk;
        }
    }

    // read decoded and compare
    {
        Base32File bf("build/tst_b32.bin", "rb");
        assert(bf.is_open());
        unsigned char *readback = new unsigned char[DATA_SIZE];
        int total = 0;
        while (total < DATA_SIZE) {
            int chunk = (DATA_SIZE - total) < BUF_SIZE
                      ? (DATA_SIZE - total) : BUF_SIZE;
            size_t n = bf.read(readback + total, chunk);
            assert((int)n == chunk);
            total += chunk;
        }
        assert(memcmp(original, readback, DATA_SIZE) == 0);
        delete[] readback;
    }

    delete[] original;
    return 0;
}

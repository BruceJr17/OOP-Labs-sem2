#include <cassert>
#include <cstdlib>
#include <cstring>
#include "../src/base_file.h"

int main() {
    const int DATA_SIZE = 41000; // > 40KB
    const int BUF_SIZE  = 256;

    // generate random data
    unsigned char *original = new unsigned char[DATA_SIZE];
    for (int i = 0; i < DATA_SIZE; i++)
        original[i] = (unsigned char)(rand() % 256);

    // write in 256-byte chunks
    {
        BaseFile bf("build/tst_base.bin", "wb");
        assert(bf.is_open());
        int written = 0;
        while (written < DATA_SIZE) {
            int chunk = (DATA_SIZE - written) < BUF_SIZE
                      ? (DATA_SIZE - written) : BUF_SIZE;
            size_t n = bf.write(original + written, chunk);
            assert((int)n == chunk);
            written += chunk;
        }
    }

    // read back in 256-byte chunks and compare
    {
        BaseFile bf("build/tst_base.bin", "rb");
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

#include <cassert>
#include <cstdlib>
#include <cstring>
#include "../src/composed_files.h"
#include "../src/base_file.h"

int main() {
    const int DATA_SIZE = 41000;
    const int BUF_SIZE  = 256;

    unsigned char *original = new unsigned char[DATA_SIZE];
    for (int i = 0; i < DATA_SIZE; i++)
        original[i] = (unsigned char)(rand() % 256);

    // ── Base32File2 wrapping BaseFile ─────────────────────────────────────
    {
        Base32File2 bf(new BaseFile("build/tst_comp_b32.bin", "wb"));
        int written = 0;
        while (written < DATA_SIZE) {
            int chunk = (DATA_SIZE - written) < BUF_SIZE
                      ? (DATA_SIZE - written) : BUF_SIZE;
            bf.write(original + written, chunk);
            written += chunk;
        }
    }
    {
        Base32File2 bf(new BaseFile("build/tst_comp_b32.bin", "rb"));
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

    // ── RleFile2 wrapping Base32File2 wrapping BaseFile ───────────────────
    {
        RleFile2 rf(new Base32File2(new BaseFile("build/tst_comp_all.bin", "wb")));
        int written = 0;
        while (written < DATA_SIZE) {
            int chunk = (DATA_SIZE - written) < BUF_SIZE
                      ? (DATA_SIZE - written) : BUF_SIZE;
            rf.write(original + written, chunk);
            written += chunk;
        }
    }
    {
        RleFile2 rf(new Base32File2(new BaseFile("build/tst_comp_all.bin", "rb")));
        unsigned char *readback = new unsigned char[DATA_SIZE];
        int total = 0;
        while (total < DATA_SIZE) {
            int chunk = (DATA_SIZE - total) < BUF_SIZE
                      ? (DATA_SIZE - total) : BUF_SIZE;
            size_t n = rf.read(readback + total, chunk);
            assert((int)n == chunk);
            total += chunk;
        }
        assert(memcmp(original, readback, DATA_SIZE) == 0);
        delete[] readback;
    }

    delete[] original;
    return 0;
}

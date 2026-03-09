#include <iostream>
#include "my_string.h"
#include "base_file.h"
#include "base32_file.h"
#include "rle_file.h"
#include "composed_files.h"
using namespace std;

int main() {
    // ── Task 1.1 ─────────────────────────────────────────────────────────
    {
        MyString ar[5] = {
            MyString("Hello"),
            MyString("World"),
            MyString("C++")
        };
        cout << "--- Task 1.1: array of MyString objects ---\n";
        for (int i = 0; i < 5; i++) {
            cout << "ar[" << i << "] = ";
            ar[i].print();
            cout << "\n";
        }
    }

    // ── Task 1.2 ─────────────────────────────────────────────────────────
    {
        MyString *arPtr[3] = {
            new MyString("First"),
            new MyString("Second"),
            new MyString("Third")
        };
        cout << "\n--- Task 1.2: array of pointers to MyString ---\n";
        for (int i = 0; i < 3; i++) {
            cout << "arPtr[" << i << "] = ";
            arPtr[i]->print();
            cout << "\n";
        }
        for (int i = 0; i < 3; i++) delete arPtr[i];
    }

    // ── Task 2.1: test BaseFile ───────────────────────────────────────────
    {
        cout << "\n--- Task 2.1: BaseFile ---\n";

        // write to a file
        BaseFile bf("build/test.txt", "w");
        cout << "is_open: " << bf.is_open() << "\n";
        const char *msg = "Hello from BaseFile!\n";
        bf.write(msg, 20);
    } // destructor closes file here

    {
        // read it back
        BaseFile bf("build/test.txt", "r");
        char buf[64] = {0};
        bf.read(buf, 63);
        cout << "Read back: " << buf;
    }

    // ── Task 2.2.1: Base32File ────────────────────────────────────────────
    {
        cout << "\n--- Task 2.2.1: Base32File ---\n";

        // write encoded data
        {
            Base32File b32("build/test_b32.txt", "w");
            const char *msg = "Hello Base32!";
            b32.write(msg, 13);
        } // flush + close on destructor

        // read and decode it back
        {
            Base32File b32("build/test_b32.txt", "r");
            char buf[64] = {0};
            size_t n = b32.read(buf, 63);
            cout << "Decoded (" << n << " bytes): " << buf << "\n";
        }
    }

    // ── Task 2.2.2: RleFile ───────────────────────────────────────────────
    {
        cout << "\n--- Task 2.2.2: RleFile ---\n";

        // ASCII art with lots of repeated characters — good for RLE
        const char *art =
            "   ###   \n"
            "  #####  \n"
            " ##   ## \n"
            "  #####  \n"
            "   ###   \n";

        int art_len = 0;
        while (art[art_len]) art_len++;

        // write compressed
        {
            RleFile rf("build/test_rle.bin", "wb");
            rf.write(art, art_len);
        }

        // read decompressed
        {
            RleFile rf("build/test_rle.bin", "rb");
            char buf[256] = {0};
            size_t n = rf.read(buf, 255);
            cout << "Decompressed (" << n << " bytes):\n" << buf;
        }

        // show compression ratio
        FILE *f = fopen("build/test_rle.bin", "rb");
        fseek(f, 0, SEEK_END);
        long compressed = ftell(f);
        fclose(f);
        cout << "Original: " << art_len << " bytes\n";
        cout << "Compressed: " << compressed << " bytes\n";
    }

    // ── Task 2.4: early binding (no virtual yet) ──────────────────────────
    {
        cout << "\n--- Task 2.4: write integer to each file type ---\n";

        int n = 123456;

        // write to plain file
        {
            BaseFile bf("build/int_base.txt", "w");
            // write each digit
            int tmp = n;
            char digits[20];
            int  len = 0;
            while (tmp > 0) { digits[len++] = '0' + (tmp % 10); tmp /= 10; }
            // digits are reversed — write them forward
            for (int i = len - 1; i >= 0; i--)
                bf.write(&digits[i], 1);
        }

        // write to base32 file
        {
            Base32File b32f("build/int_b32.txt", "w");
            int tmp = n;
            char digits[20];
            int  len = 0;
            while (tmp > 0) { digits[len++] = '0' + (tmp % 10); tmp /= 10; }
            for (int i = len - 1; i >= 0; i--)
                b32f.write(&digits[i], 1);
        }

        // write to rle file
        {
            RleFile rf("build/int_rle.bin", "wb");
            int tmp = n;
            char digits[20];
            int  len = 0;
            while (tmp > 0) { digits[len++] = '0' + (tmp % 10); tmp /= 10; }
            for (int i = len - 1; i >= 0; i--)
                rf.write(&digits[i], 1);
        }

        cout << "Written 123456 to all three file types.\n";
        cout << "Notice: code was repeated 3 times — Task 2.5 fixes this.\n";
    }

    // ── Task 2.5: write_int function using BaseFile reference ─────────────
    // ── Task 2.6: virtual functions fix the behavior ──────────────────────
    {
        cout << "\n--- Task 2.5 + 2.6: write_int via BaseFile reference ---\n";

        // This function works for ALL subclasses because write() is virtual.
        // Without virtual: bf.write() would ALWAYS call BaseFile::write()
        //   even when passed a Base32File or RleFile — wrong behavior.
        // With virtual:    the correct subclass write() is called — correct.
        auto write_int = [](BaseFile &file, int n) {
            if (n < 0) { char c = '-'; file.write(&c, 1); n = -n; }
            char digits[20];
            int  len = 0;
            while (n > 0) { digits[len++] = '0' + (n % 10); n /= 10; }
            if (len == 0)  { char c = '0'; file.write(&c, 1); return; }
            for (int i = len - 1; i >= 0; i--)
                file.write(&digits[i], 1);
        };

        // same function, three different file types
        {
            BaseFile bf("build/wint_base.txt", "w");
            write_int(bf, 123456);
        }
        {
            Base32File b32f("build/wint_b32.txt", "w");
            write_int(b32f, 123456);   // calls Base32File::write — correct!
        }
        {
            RleFile rf("build/wint_rle.bin", "wb");
            write_int(rf, 123456);     // calls RleFile::write — correct!
        }

        // verify base file result
        {
            BaseFile bf("build/wint_base.txt", "r");
            char buf[32] = {0};
            bf.read(buf, 31);
            cout << "BaseFile result:   " << buf << "\n";
        }
        // verify base32 result
        {
            Base32File b32f("build/wint_b32.txt", "r");
            char buf[32] = {0};
            b32f.read(buf, 31);
            cout << "Base32File result: " << buf << "\n";
        }
        // verify rle result
        {
            RleFile rf("build/wint_rle.bin", "rb");
            char buf[32] = {0};
            rf.read(buf, 31);
            cout << "RleFile result:    " << buf << "\n";
        }
    }

    // ── Task 2.7: virtual destructor ──────────────────────────────────────
    {
        cout << "\n--- Task 2.7: virtual destructor ---\n";
        // Without virtual destructor: delete calls ONLY BaseFile::~BaseFile
        // This leaks RleFile/Base32File resources and skips their flush_write!
        // With virtual destructor: correct subclass destructor is called first.

        BaseFile *files[] = {
            new BaseFile("build/vd_base.txt", "w"),
            new RleFile ("build/vd_rle.bin",  "wb"),
            new Base32File("build/vd_b32.txt","w"),
        };

        for (int i = 0; i < 3; i++) {
            const char *msg = "Hello!";
            files[i]->write(msg, 6);
        }

        // delete through base pointer — virtual destructor ensures
        // the correct derived destructor runs first
        for (int i = 0; i < 3; i++) {
            delete files[i];
        }
    }

    // ── Task 2.8: array of derived objects — BAD PRACTICE ─────────────────
    {
        cout << "\n--- Task 2.8: array of base pointers (correct way) ---\n";
        // BAD: BaseFile *arr = new Base32File[2] — pointer arithmetic breaks
        //      because sizeof(BaseFile) != sizeof(Base32File)
        //
        // CORRECT: use array of BASE POINTERS to derived objects
        BaseFile *arr[2] = {
            new Base32File("build/arr0.txt", "w"),
            new Base32File("build/arr1.txt", "w"),
        };
        for (int i = 0; i < 2; i++) {
            const char *msg = "Hello!";
            arr[i]->write(msg, 6);  // calls Base32File::write correctly
        }
        for (int i = 0; i < 2; i++) delete arr[i];
    }

    // ── Task 3.1: IFile interface ─────────────────────────────────────────
    // write_int now works with ANY IFile, not just BaseFile
    auto write_int = [](IFile &file, int n) {
        if (n < 0) { char c = '-'; file.write(&c, 1); n = -n; }
        char digits[20]; int len = 0;
        while (n > 0) { digits[len++] = '0' + (n % 10); n /= 10; }
        if (len == 0)  { char c = '0'; file.write(&c, 1); return; }
        for (int i = len - 1; i >= 0; i--) file.write(&digits[i], 1);
    };

    // ── Task 3.2: composition ─────────────────────────────────────────────
    {
        cout << "\n--- Task 3.2: composition ---\n";

        // Base32File2 wraps a BaseFile
        {
            Base32File2 b32f(new BaseFile("build/comp_b32.txt", "w"));
            write_int(b32f, 123456);
        }
        {
            Base32File2 b32f(new BaseFile("build/comp_b32.txt", "r"));
            char buf[32] = {0};
            b32f.read(buf, 31);
            cout << "Base32File2 result: " << buf << "\n";
        }

        // RleFile2 wraps a BaseFile
        {
            RleFile2 rf(new BaseFile("build/comp_rle.bin", "wb"));
            write_int(rf, 123456);
        }
        {
            RleFile2 rf(new BaseFile("build/comp_rle.bin", "rb"));
            char buf[32] = {0};
            rf.read(buf, 31);
            cout << "RleFile2 result:    " << buf << "\n";
        }
    }

    // ── Task 3.3: double base32 + RLE composition ─────────────────────────
    {
        cout << "\n--- Task 3.3: Base32 -> Base32 -> RLE -> File ---\n";

        // write: data → base32 → base32 → rle → file
        {
            RleFile2 rf(
                new Base32File2(
                    new Base32File2(
                        new BaseFile("build/comp_all.bin", "wb")
                    )
                )
            );
            write_int(rf, 123456);
        }

        // read back the same way
        {
            RleFile2 rf(
                new Base32File2(
                    new Base32File2(
                        new BaseFile("build/comp_all.bin", "rb")
                    )
                )
            );
            char buf[64] = {0};
            rf.read(buf, 63);
            std::cout << "Composed result: " << buf << "\n";
        }
    }

    return 0;
}

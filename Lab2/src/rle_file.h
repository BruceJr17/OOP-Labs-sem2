#pragma once
#include "base_file.h"

class RleFile : public BaseFile {
private:
    // write state
    unsigned char write_last_byte;
    int           write_count;
    bool          write_started;

    // read state
    unsigned char read_current_byte;
    int           read_remaining;

    void flush_write();

public:
    RleFile();
    RleFile(const char *path, const char *mode);
    RleFile(FILE *f);
    ~RleFile();

    size_t write(const void *buf, size_t n_bytes) override;
    size_t read(void  *buf, size_t max_bytes)     override;
};

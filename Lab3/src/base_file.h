#pragma once
#include <cstdio>
#include "ifile.h"

class BaseFile : public IFile {
protected:
      FILE *file;

public:
      // constructors
      BaseFile();
      BaseFile(const char *path, const char *mode);
      BaseFile(FILE *f);

    // Task 5: copy constructor and copy assignment are deleted
    // because two objects owning the same FILE* would double-close it
    BaseFile(const BaseFile&)            = delete;
    BaseFile& operator=(const BaseFile&) = delete;

    // move constructor and move assignment are allowed
    // they transfer ownership of the FILE* pointer
    BaseFile(BaseFile&& other);
    BaseFile& operator=(BaseFile&& other);

       // destructor closes the file
       virtual ~BaseFile();

       // status
       bool is_open()    const;
       bool can_read()   const;
       bool can_write()  const;

       //raw read/write (always go directly to file)
       size_t write_raw(const void *buf, size_t n_bytes);
       size_t read_raw(void *buf, size_t max_bytes);

       // read/write (can be overridden by subclasses)
       virtual size_t write(const void *buf, size_t n_byytes);
       virtual size_t read(void *buf, size_t max_bytes);

       // file position
       long tell() const;
       bool seek(long offset);
};

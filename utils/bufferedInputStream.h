#ifndef _BUFFERED_INPUT_STREAM_H
#define _BUFFERED_INPUT_STREAM_H

#include <cstdio>

#define BUFFER_LEN 256

/* a utility to read input file */
class BufferedInputStream {
private:
    FILE* fp;
    char buffer[BUFFER_LEN];
    unsigned int index;
    unsigned int index_lim;
public:
    BufferedInputStream(char const* filename);

    ~BufferedInputStream();

    unsigned char read();

    int read_int();

    void unread();
};

#endif

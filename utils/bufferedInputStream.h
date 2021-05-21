#ifndef _BUFFERED_INPUT_STREAM_H
#define _BUFFERED_INPUT_STREAM_H

#include <cstdio>
#include <iostream>

using namespace std;

#define BUFFER_LEN 256

class BufferedInputStream {
private:
    FILE* fp;
    char buffer[BUFFER_LEN];
    unsigned int index;
    unsigned int index_lim;
public:
    BufferedInputStream(char const* filename) {
        fp = fopen(filename, "rb");
        if(fp == nullptr) {
            cerr << "open file error [" << filename << "]" << endl;
            exit(-1);
        }
        index_lim = fread(buffer, 1, BUFFER_LEN, fp);
        index = 0;
    }

    ~BufferedInputStream() {
        if(fp != nullptr)
            fclose(fp);
        fp = nullptr;
    }

    unsigned char read() {
        if(index < index_lim && index < BUFFER_LEN) {
            return buffer[index++];
        }
        else if(index_lim < BUFFER_LEN) {
            cerr << "unexpected eof" << endl;
            exit(-1);
        }
        else {
            index = 0;
            index_lim = fread(buffer, BUFFER_LEN, 1, fp);
            return buffer[index++];
        }
    }

    int read_int() {
        char bytes[4];
        bytes[0] = read();
        bytes[1] = read();
        bytes[2] = read();
        bytes[3] = read();
        return *reinterpret_cast<int*>(&bytes);
    }
};

#endif

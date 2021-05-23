#include "bufferedInputStream.h"

#include <iostream>

using namespace std;


BufferedInputStream::BufferedInputStream(char const* filename) {
    fp = fopen(filename, "rb");
    if(fp == nullptr) {
        cerr << "open file error [" << filename << "]" << endl;
        exit(-1);
    }
    index_lim = fread(buffer, 1, BUFFER_LEN, fp);
    index = 0;
}

BufferedInputStream::~BufferedInputStream() {
    if(fp != nullptr)
        fclose(fp);
    fp = nullptr;
}

unsigned char BufferedInputStream::read() {
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

int BufferedInputStream::read_int() {
    char bytes[4];
    bytes[0] = read();
    bytes[1] = read();
    bytes[2] = read();
    bytes[3] = read();
    return *reinterpret_cast<int*>(&bytes);
}

void BufferedInputStream::unread() {
    index--;
}


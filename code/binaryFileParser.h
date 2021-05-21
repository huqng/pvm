#ifndef _BINARY_FILE_PARSER_H
#define _BINARY_FILE_PARSER_H

#include "../utils/bufferedInputStream.h"
#include "codeObjet.h"

class BinaryFileParser {
private:
    BufferedInputStream* fs;

    CodeObject* get_code_object();
public:
    BinaryFileParser(BufferedInputStream* s);
    CodeObject* parse();
};

#endif

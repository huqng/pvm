#ifndef _BINARY_FILE_PARSER_H
#define _BINARY_FILE_PARSER_H

#include "bufferedInputStream.h"
#include "arrayList.h"

class BufferedInputStream;
class CodeObject;
class StringObject;

class BinaryFileParser {
private:
    BufferedInputStream* is;
    ObjList*        _string_table;

    CodeObject*     get_code_object();
    StringObject*   get_byte_code();
    ObjList*        get_consts();
    ObjList*        get_names();
    ObjList*        get_var_names();
    ObjList*        get_free_vars();
    ObjList*        get_cell_vars();
    StringObject*   get_file_name();
    StringObject*   get_module_name();
    StringObject*   get_no_table();
    StringObject*   get_string();
    ObjList*        get_tuple();

public:
    BinaryFileParser(BufferedInputStream* s);
    CodeObject* parse();
};

#endif

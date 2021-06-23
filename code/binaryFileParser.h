#ifndef _BINARY_FILE_PARSER_H
#define _BINARY_FILE_PARSER_H

#include "bufferedInputStream.h"

class BufferedInputStream;
class CodeObject;
class StringObject;
class ListObject;

class BinaryFileParser {
private:
    BufferedInputStream* is;
    ListObject*        _string_table;

    CodeObject*     get_code_object();
    StringObject*   get_byte_code();
    ListObject*     get_consts();
    ListObject*     get_names();
    ListObject*     get_var_names();
    ListObject*     get_free_vars();
    ListObject*     get_cell_vars();
    StringObject*   get_file_name();
    StringObject*   get_module_name();
    StringObject*   get_no_table();
    StringObject*   get_string();
    ListObject*     get_tuple();

public:
    BinaryFileParser(BufferedInputStream* s);
    CodeObject* parse();
};

#endif

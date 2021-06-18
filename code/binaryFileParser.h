#ifndef _BINARY_FILE_PARSER_H
#define _BINARY_FILE_PARSER_H

#include "bufferedInputStream.h"
#include "codeObject.h"
#include "universe.h"

class BinaryFileParser {
private:
    BufferedInputStream* is;
    ArrayList<Object*>* _string_table;

    CodeObject*             get_code_object();
    StringObject*           get_byte_code();
    ArrayList<Object*>*    get_consts();
    ArrayList<Object*>*    get_names();
    ArrayList<Object*>*    get_var_names();
    ArrayList<Object*>*    get_free_vars();
    ArrayList<Object*>*    get_cell_vars();
    StringObject*           get_file_name();
    StringObject*           get_module_name();
    StringObject*           get_no_table();
    StringObject*           get_string();
    ArrayList<Object*>*    get_tuple();

public:
    BinaryFileParser(BufferedInputStream* s);
    CodeObject* parse();
};

#endif

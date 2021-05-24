#ifndef _BINARY_FILE_PARSER_H
#define _BINARY_FILE_PARSER_H

#include "../utils/bufferedInputStream.h"
#include "codeObject.h"

class BinaryFileParser {
private:
    BufferedInputStream* fs;
    ArrayList<PObject*>* _string_table;

    CodeObject* get_code_object();

    StringObject* get_byte_code();
    ArrayList<PObject*>* get_consts();
    ArrayList<PObject*>* get_names();
    ArrayList<PObject*>* get_var_names();
    ArrayList<PObject*>* get_free_vars();
    ArrayList<PObject*>* get_cell_vars();
    StringObject* get_file_name();
    StringObject* get_module_name();
    StringObject* get_no_table();

    StringObject* get_string();
    ArrayList<PObject*>* get_tuple();
public:
    BinaryFileParser(BufferedInputStream* s);
    CodeObject* parse();
};

#endif

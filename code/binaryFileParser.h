#ifndef _BINARY_FILE_PARSER_H
#define _BINARY_FILE_PARSER_H

#include "../utils/bufferedInputStream.h"
#include "codeObjet.h"

class BinaryFileParser {
private:
    BufferedInputStream* fs;
    ArrayList<PObject*>* _string_table;

    CodeObject* get_code_object();

    PString* get_byte_code();
    ArrayList<PObject*>* get_consts();
    ArrayList<PObject*>* get_names();
    ArrayList<PObject*>* get_var_names();
    ArrayList<PObject*>* get_free_vars();
    ArrayList<PObject*>* get_cell_vars();
    PString* get_file_name();
    PString* get_module_name();
    PString* get_no_table();

    PString* get_string();
    ArrayList<PObject*>* get_tuple();
public:
    BinaryFileParser(BufferedInputStream* s);
    CodeObject* parse();
};

#endif

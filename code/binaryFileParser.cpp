#include "binaryFileParser.h"

BinaryFileParser::BinaryFileParser(BufferedInputStream* s): fs(s) {

}

CodeObject* BinaryFileParser::parse() {
    int magic_number = fs->read_int();
    int moddate = fs->read_int();
    char obj_type = fs->read();

    cout << "Magic number: " << hex << magic_number << endl;
    cout << "Moddate:      " << hex << moddate << endl;

    if(obj_type == 'c') {
        CodeObject* result = get_code_object();
        return result;
    }
    return nullptr;
}


    CodeObject* BinaryFileParser::get_code_object() {
        return NULL;
    }
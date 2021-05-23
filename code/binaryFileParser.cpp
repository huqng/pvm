#include "binaryFileParser.h"
#include "../object/pInteger.h"
#include "../runtime/universe.h"

#include <iomanip>

BinaryFileParser::BinaryFileParser(BufferedInputStream* s): fs(s) {
    _string_table = new ArrayList<PObject*>();
}

CodeObject* BinaryFileParser::parse() {
    int magic_number = fs->read_int();
    int moddate = fs->read_int();
    char obj_type = fs->read();

    cout << setiosflags(ios::left) << hex;
    cout << setw(16) << "Magic number: " << "0x" << magic_number << endl;
    cout << setw(16) << "Moddate: " << "0x" << moddate << endl;

    if(obj_type == 'c') {
        cout << setw(16) << "Obj-type:" << "[c]" << endl;
        CodeObject* result = get_code_object();

        result->print();

        return result;
    }
    cout << resetiosflags(ios::fmtflags::_S_ios_fmtflags_max);
    return nullptr;
}


CodeObject* BinaryFileParser::get_code_object() {
    int argcount = fs->read_int();
    int nlocals = fs->read_int();
    int stacksize = fs->read_int();
    int flags = fs->read_int();

    PString* bytecode = get_byte_code();
    ArrayList<PObject*>*consts = get_consts();
    ArrayList<PObject*>*names = get_names();
    ArrayList<PObject*>*var_names = get_var_names();
    ArrayList<PObject*>*free_vars = get_free_vars();
    ArrayList<PObject*>*cell_vars = get_cell_vars();

    PString* file_name = get_file_name();
    PString* module_name = get_module_name();
    int lineno = fs->read_int();
    PString* notable = get_no_table();
    return new CodeObject(
        argcount,
        nlocals,
        stacksize,
        flags,
        bytecode,
        consts,
        names,
        var_names,
        free_vars,
        cell_vars,
        file_name,
        module_name,
        lineno,
        notable
    );
}

PString* BinaryFileParser::get_byte_code() {
    char obj_type = fs->read();
    if(obj_type == 's') {
        return get_string();
    }
    else if(obj_type == 't') {
        PString* s = get_string();
        _string_table->add(s);
        return s;
    }
    else {
        cerr << "parse error: " << obj_type << endl;
        exit(-1);
    }
}

ArrayList<PObject*>* BinaryFileParser::get_consts() {
    if(fs->read() == '(')
        return get_tuple();
    fs->unread();
    return NULL;
}

ArrayList<PObject*>* BinaryFileParser::get_names() {
    return get_consts();
}

ArrayList<PObject*>* BinaryFileParser::get_var_names() {
    return get_consts();
}

ArrayList<PObject*>* BinaryFileParser::get_free_vars() {
    return get_consts();
}

ArrayList<PObject*>* BinaryFileParser::get_cell_vars() {
    return get_consts();
}

PString* BinaryFileParser::get_file_name() {
    return get_byte_code();
}

PString* BinaryFileParser::get_module_name() {
    return get_byte_code();
}

PString* BinaryFileParser::get_no_table() {
    return get_byte_code();
}

PString* BinaryFileParser::get_string() {
    int len = fs->read_int();
    char* buf = new char[len];
    for(int i = 0; i < len; i++)
        buf[i] = fs->read();
    PString* s =  new PString(buf, len);
    delete[] buf;
    return s;
}

ArrayList<PObject*>* BinaryFileParser::get_tuple() {
    int length = fs->read_int();
    PString* str;
    ArrayList<PObject*>* tuple = new ArrayList<PObject*>(length);
    for(int i = 0; i < length; i++) {
        char obj_type = fs->read();
        switch (obj_type) {
        case 'c':
            tuple->add(get_code_object());
            break;
        case 'i':
            tuple->add(new PInteger(fs->read_int()));
            break;
        case 'N':
            tuple->add(Universe::PNone);
            break;
        case 't':
            str = get_string();
            tuple->add(str);
            _string_table->add(str);
            break;
        case 's':
            tuple->add(get_string());
            break;
        case 'R':
            tuple->add(_string_table->get(fs->read_int()));
            break;
        default:
            cerr << "parse error: obj_type" << endl;
            break;
        }
    }
    return tuple;
}

#include "binaryFileParser.h"

#include <iomanip>

BinaryFileParser::BinaryFileParser(BufferedInputStream* s): is(s) {
    _string_table = new ArrayList<PObject*>();
}

CodeObject* BinaryFileParser::parse() {
    int magic_number = is->read_int();
    int moddate = is->read_int();
    char obj_type = is->read();

    cout << setiosflags(ios::left) << hex;
    cout << setw(20) << "Magic number: " << "0x" << magic_number << endl;
    cout << setw(20) << "Moddate: " << "0x" << moddate << endl;

    if(obj_type == 'c') {
        cout << setw(20) << "Obj-type:" << "\'c\'" << endl;
        
        CodeObject* result = get_code_object();

        result->print_details();

        return result;
    }
    else {
        cout << "fail to parse" << endl;
        exit(-1);
    }
}


CodeObject* BinaryFileParser::get_code_object() {
    int argcount = is->read_int();
    int nlocals = is->read_int();
    int stacksize = is->read_int();
    int flags = is->read_int();

    StringObject* bytecode = get_byte_code();
    ArrayList<PObject*>*consts = get_consts();
    ArrayList<PObject*>*names = get_names();
    ArrayList<PObject*>*var_names = get_var_names();
    ArrayList<PObject*>*free_vars = get_free_vars();
    ArrayList<PObject*>*cell_vars = get_cell_vars();

    StringObject* file_name = get_file_name();
    StringObject* module_name = get_module_name();
    int lineno = is->read_int();
    StringObject* notable = get_no_table();
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

StringObject* BinaryFileParser::get_byte_code() {
    char obj_type = is->read();
    if(obj_type == 's') {
        return get_string();
    }
    else if(obj_type == 't') {
        StringObject* s = get_string();
        _string_table->add(s);
        return s;
    }
    else if(obj_type == 'R') {
        return (StringObject*)_string_table->get(is->read_int());
    }
    else {
        cerr << "parse error: " << obj_type << endl;
        exit(-1);
    }
}

ArrayList<PObject*>* BinaryFileParser::get_consts() {
    if(is->read() == '(')
        return get_tuple();
    is->unread();
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

StringObject* BinaryFileParser::get_file_name() {
    return get_byte_code();
}

StringObject* BinaryFileParser::get_module_name() {
    return get_byte_code();
}

StringObject* BinaryFileParser::get_no_table() {
    return get_byte_code();
}

StringObject* BinaryFileParser::get_string() {
    int len = is->read_int();
    char* buf = new char[len];
    for(int i = 0; i < len; i++)
        buf[i] = is->read();
    StringObject* s =  new StringObject(buf, len);
    delete[] buf;
    return s;
}

ArrayList<PObject*>* BinaryFileParser::get_tuple() {
    int length = is->read_int();
    StringObject* str;
    ArrayList<PObject*>* tuple = new ArrayList<PObject*>();
    for(int i = 0; i < length; i++) {
        char obj_type = is->read();
        switch (obj_type) {
        case 'c':
            tuple->add(get_code_object());
            break;
        case 'i':
            tuple->add(new integerObject(is->read_int()));
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
            tuple->add(_string_table->get(is->read_int()));
            break;
        default:
            cerr << "parse error: obj_type" << endl;
            break;
        }
    }
    return tuple;
}

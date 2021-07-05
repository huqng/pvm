#include "binaryFileParser.h"
#include "interpreter.h"
#include "universe.h"
#include "listObject.h"
#include "codeObject.h"
#include <getopt.h>

int main(int argc, char** argv) {
    int debug = 0;

    const char* optstr = "d";
    int opt;
    while(opt = getopt(argc, argv, optstr)) {
        switch (opt) {
        case 'd':
            debug = 1;
            break;
        
        default:
            break;
        }
        if(opt < 0)
            break;
    }

    /* init environment */
    Universe::genesis();

    /* a file-read-util */
    BufferedInputStream* bis = new BufferedInputStream("../.pytest/hello.pyc");

    /* .pyc file parser */
    BinaryFileParser* bfp = new BinaryFileParser(bis);


    /* run interpreter */
    Interpreter* interpreter = Interpreter::get_instance();
    interpreter->set_debug(debug);

    /* parse and get codeobject */
    Universe::main_code = bfp->parse();
    Universe::gc();

    interpreter->run(Universe::main_code);

    Universe::destory();

    return 0;
}
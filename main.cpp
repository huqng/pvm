#include "binaryFileParser.h"
#include "interpreter.h"
#include "universe.h"

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

    /* a file-read-util */
    BufferedInputStream* bis = new BufferedInputStream("../.pytest/hello.pyc");

    /* .pyc file parser */
    BinaryFileParser* bfp = new BinaryFileParser(bis);

    /* parse and get codeobject */
    CodeObject* co = bfp->parse();

    /* init environment */
    Universe::genesis();

    /* run interpreter */
    Interpreter* interpreter = Interpreter::get_instance();
    interpreter->set_debug(debug);
    interpreter->run(co);


    return 0;
}
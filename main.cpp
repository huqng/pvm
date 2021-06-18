#include "binaryFileParser.h"
#include "interpreter.h"

int main() {
    /* a file-read-util */
    BufferedInputStream* bis = new BufferedInputStream("../.pytest/hello.pyc");

    /* .pyc file parser */
    BinaryFileParser* bfp = new BinaryFileParser(bis);

    /* parse and get codeobject */
    CodeObject* co = bfp->parse();

    /* init environment */
    Universe::genesis();

    /* run interpreter */
    Interpreter* interpreter = new Interpreter();
    interpreter->run(co);


    return 0;
}
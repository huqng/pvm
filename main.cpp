#include "code/binaryFileParser.h"
#include "runtime/interpreter.h"

int main() {
    /* a file-read-util */
    BufferedInputStream* bis = new BufferedInputStream("../.pytest/hello.pyc");

    /* .pyc file parser */
    BinaryFileParser* bfp = new BinaryFileParser(bis);

    /* get codeobject */
    CodeObject* co = bfp->parse();

    /* init environment */
    Universe::genesis();

    /* run interpreter */
    Interpreter interpreter;
    interpreter.run(co);


    return 0;
}
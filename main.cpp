#include "code/binaryFileParser.h"
#include "utils/bufferedInputStream.h"
#include "runtime/interpreter.h"
#include "runtime/universe.h"

int main() {
    /* a file-read-util */
    BufferedInputStream* bis = new BufferedInputStream("../.py/hello.pyc");

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
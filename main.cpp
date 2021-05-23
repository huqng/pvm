#include "code/binaryFileParser.h"
#include "utils/bufferedInputStream.h"
#include "runtime/interpreter.h"

int main() {
    BufferedInputStream* bis = new BufferedInputStream("../.py/hello.pyc");
    BinaryFileParser* bfp = new BinaryFileParser(bis);

    CodeObject* co = bfp->parse();

    Interpreter interpreter;
    interpreter.run(co);


    return 0;
}
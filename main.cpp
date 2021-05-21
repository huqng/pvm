#include "code/binaryFileParser.h"

#include "utils/bufferedInputStream.h"

int main() {
    BufferedInputStream* bis = new BufferedInputStream("../.py/hello.pyc");
    BinaryFileParser* bfp = new BinaryFileParser(bis);

    bfp->parse();

    return 0;
}
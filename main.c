#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "huffman.h"
#include "utils/pqueue.h"
#include "utils/bitio.h"

int main(int argc, char **argv) {

  encode("../huckfinn.txt", "out.bin");

  decode("out.bin");

  return 0;
}

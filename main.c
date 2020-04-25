#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "huffman.h"
#include "utils/pqueue.h"
#include "utils/bitio.h"

int main(int argc, char **argv) {

  int bytes_encoded, bytes_decoded;

  bytes_encoded = encode("../huckfinn.txt", "out.bin");

  bytes_decoded = decode("out.bin", "huckfinn.txt");

  assert(bytes_encoded == bytes_decoded);

  return 0;
}

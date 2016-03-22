#include <z3++.h>
#include "loader.h"
#include "solver.h"

word32 crc32(word32 state, word8 byte) {
  /* http://www.hackersdelight.org/hdcodetxt/crc.c.txt */
  word32 crc = state ^ word32(byte);
  for (int j = 7; j >= 0; j--) {
    word32 mask = -(crc & word32(1));
    crc = (crc >> 1) ^ (mask & word32(0xEDB88320));
  }
  return crc;
}

int main(int argc, char* argv[]) {

  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " <zip_file>" << std::endl;
    return 0;
  }

  std::vector<encrypted_file> files = load_file(argv[1]);

  //solver s;

  //word32 state("state");
  //word32 out = crc32(state, word8(0xa5)) == word32(0xdeadbeef);

  //s.add(out);

  //while (s.solve()) {
    //s.print();
    //s.next();
  //}

  //s = solver();

  //word32 x("x");
  //word32 ret = crc32(word32(0x16202a48), word8(0xa5)) == x;

  //s.add(ret);

  //while (s.solve()) {
    //s.print();
    //s.next();
  //}

  return 0;
}

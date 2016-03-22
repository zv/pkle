#include <z3++.h>
#include "solver.h"

int main() {
  word8 x("x");
  word8 y(127);

  solver s;

  word8 r = x >> 1 == y;

  s.add(r);

  while (s.solve()) {
    s.print();
    s.next();
  }

  return 0;
}

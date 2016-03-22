#include "z3++.h"
#include "word.h"

using namespace z3;

int main() {
  word8 x("x");
  word8 y(127);

  solver s(z3_context());

  word8 r = x >> 1 == y;

  s.add(r.expr());

  std::cout << s << "\n";
  std::cout << s.check() << "\n";
  std::cout << s.get_model().num_consts() << std::endl;
  std::cout << s.get_model().num_funcs() << std::endl;
  std::cout << s.get_model() << "\n";

  return 0;
}

#include "context.h"

z3::context& z3_context() {
  static z3::context context;
  return context;
}

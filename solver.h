#ifndef __SOLVER_H__
#define __SOLVER_H__

#include <z3++.h>
#include <vector>
#include "context.h"
#include "word.h"

class solver {
public:

  solver() : solver_(z3_context()) {
    solver_.push();
  }

  template <size_t N> void add(const word<N>& word) {
    solver_.add(word.expr());
  }

  bool solve() {
    return solver_.check() == z3::sat;
  }

  void next() {
    z3::model model = solver_.get_model();
    for (int i = 0; i < model.num_consts(); ++i) {
      z3::func_decl decl = model.get_const_decl(i);
      std::string name = decl.name().str();
      z3::expr constant = model.get_const_interp(decl);
      size_t bv_size = constant.get_sort().bv_size();
      z3::expr var = z3_context().bv_const(name.c_str(), bv_size);
      z3::expr not_equal = var != constant;
      solver_.add(not_equal);
    }
  }

  typedef std::pair<std::string, uint64_t> solution;

  std::vector<solution> solutions() {
    std::vector<solution> sol;
    z3::model model = solver_.get_model();
    for (int i = 0; i < model.num_consts(); ++i) {
      z3::func_decl decl = model.get_const_decl(i);
      z3::expr constant = model.get_const_interp(decl);
      __uint64 u = 0;
      Z3_bool ret = Z3_get_numeral_uint64(z3_context(), constant, &u);
      if (ret == Z3_TRUE) {
        std::string name = decl.name().str();
        sol.push_back(std::make_pair(name, u));
      }
    }
    return sol;
  }

  void print() {
    z3::model model = solver_.get_model();
    for (int i = 0; i < model.num_consts(); ++i) {
      z3::func_decl decl = model.get_const_decl(i);
      z3::expr constant = model.get_const_interp(decl);
      __uint64 u = 0;
      Z3_bool ret = Z3_get_numeral_uint64(z3_context(), constant, &u);
      if (ret == Z3_TRUE) {
        std::cout << decl.name() << " = " << char(u) << std::endl;
      }
    }
  }

private:

  z3::solver solver_;

};

#endif /* __SOLVER_H__ */

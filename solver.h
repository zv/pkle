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
    std::vector<solution> ret;
    z3::model model = solver_.get_model();
    for (int i = 0; i < model.num_consts(); ++i) {
      z3::func_decl decl = model.get_const_decl(i);
      word64 constant = model.get_const_interp(decl);
      boost::optional<uint64_t> v = constant.value();
      if (v) {
        std::string name = decl.name().str();
        ret.push_back(std::make_pair(name, *v));
      }
    }
    return ret;
  }

  void print() {
    z3::model model = solver_.get_model();
    for (int i = 0; i < model.num_consts(); ++i) {
      z3::func_decl decl = model.get_const_decl(i);
      word64 constant = model.get_const_interp(decl);
      boost::optional<uint64_t> v = constant.value();
      if (v) {
        std::cout << decl.name() << " = " << std::hex << *v << std::endl;
      }
    }
  }

private:

  z3::solver solver_;

};

#endif /* __SOLVER_H__ */

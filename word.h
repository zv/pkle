#ifndef __WORD_H__
#define __WORD_H__

#include <z3++.h>
#include <stdint.h>
#include <boost/optional.hpp>
#include "context.h"

template <size_t N> class word {
public:

  word() : expr_(z3_context()) {
  }

  word(z3::expr expr) : expr_(expr) {
  }

  word(const word& src) : expr_(z3_context()) {
    *this = src;
  }

  explicit word(const std::string& name) : expr_(z3_context()) {
    expr_ = z3_context().bv_const(name.c_str(), N);
  }

  explicit word(const uint64_t value) : expr_(z3_context()) {
    expr_ = z3_context().bv_val((__uint64)value, N);
  }

  template <size_t M> explicit word(const word<M>& src) : expr_(z3_context()) {
    if (N < M) {
      expr_ = src.expr().extract(N-1, 0);
    } else {
      Z3_ast ext = Z3_mk_zero_ext(z3_context(), unsigned(N-M), src.expr());
      expr_ = z3::to_expr(z3_context(), ext);
    }
  }

  word& operator =(const word& rhs) {
    expr_ = rhs.expr_;
    return *this;
  }

  word& operator =(const z3::expr& rhs) {
    expr_ = rhs;
    return *this;
  }

  word operator -() {
    return -expr_;
  }

  word operator ==(const word& rhs) {
    return expr_ == rhs.expr_;
  }

  word operator !=(const word& rhs) {
    return expr_ != rhs.expr_;
  }

  word operator <<(const int rhs) {
    word sa(rhs);
    return z3::to_expr(z3_context(), Z3_mk_bvshl(z3_context(), expr_, sa.expr()));
  }

  word& operator <<=(const int rhs) {
    *this = *this << rhs;
    return *this;
  }

  word operator >>(const int rhs) {
    word sa(rhs);
    return z3::to_expr(z3_context(), Z3_mk_bvlshr(z3_context(), expr_, sa.expr()));
  }

  word& operator >>=(const int rhs) {
    *this = *this >> rhs;
    return *this;
  }

  word operator &(const word& rhs) {
    return expr_ & rhs.expr_;
  }

  word& operator &=(const word& rhs) {
    *this = *this & rhs;
    return *this;
  }

  word operator |(const word& rhs) {
    return expr_ | rhs.expr_;
  }

  word& operator |=(const word& rhs) {
    *this = *this | rhs;
    return *this;
  }

  word operator ^(const word& rhs) {
    return expr_ ^ rhs.expr_;
  }

  word& operator ^=(const word& rhs) {
    *this = *this ^ rhs;
    return *this;
  }

  word operator +(const word& rhs) {
    return expr_ + rhs.expr_;
  }

  word& operator +=(const word& rhs) {
    *this = *this + rhs;
    return *this;
  }

  word operator -(const word& rhs) {
    return expr_ - rhs.expr_;
  }

  word& operator -=(const word& rhs) {
    *this = *this - rhs;
    return *this;
  }

  word operator *(const word& rhs) {
    return expr_ * rhs.expr_;
  }

  word& operator *=(const word& rhs) {
    *this = *this * rhs;
    return *this;
  }

  z3::expr expr() const {
    return expr_;
  }

private:

  z3::expr expr_;

};

typedef word<1> word1;
typedef word<2> word2;
typedef word<4> word4;
typedef word<8> word8;
typedef word<16> word16;
typedef word<32> word32;
typedef word<64> word64;

#endif /* __WORD_H__ */

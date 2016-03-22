#ifndef __WORD_H__
#define __WORD_H__

#include <z3++.h>
#include <stdint.h>
#include <boost/optional.hpp>
#include "context.h"

template <size_t N> class word {
public:

  explicit word(const std::string& name) : expr_(z3_context()) {
    expr_ = z3_context().bv_const(name.c_str(), N);
  }

  explicit word(const uint64_t value) : expr_(z3_context()) {
    expr_ = z3_context().bv_val((__uint64)value, N);
  }

  word(const word& src) : expr_(z3_context()) {
    *this = src;
  }

  word(z3::expr expr) :expr_(expr) {
  }

  word& operator =(const word& rhs) {
    expr_ = rhs.expr_;
    return *this;
  }

  word& operator =(const z3::expr& rhs) {
    expr_ = rhs;
    return *this;
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

  boost::optional<uint64_t> value() {
    __uint64 u = 0;
    Z3_bool ret = Z3_get_numeral_uint64(z3_context(), expr_, &u);
    if (ret == Z3_TRUE) {
      return boost::optional<uint64_t>(u);
    } else {
      return boost::optional<uint64_t>();
    }
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

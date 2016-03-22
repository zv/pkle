#include <z3++.h>
#include <sstream>
#include "loader.h"
#include "solver.h"

word32 crc32(word32 state, const word8& byte) {
  /* http://www.hackersdelight.org/hdcodetxt/crc.c.txt */
  word32 mask;
  word32 crc = state ^ word32(byte);
  for (int j = 7; j >= 0; j--) {
     mask = -(crc & word32(1));
     crc = (crc >> 1) ^ (mask & word32(0xEDB88320));
  }
  return crc;
}

struct cipher_state {
  cipher_state() : state0(0), state1(0), state2(0) {
  }
  cipher_state(const cipher_state& src) : state0(0), state1(0), state2(0) {
    *this = src;
  }
  cipher_state& operator =(const cipher_state& rhs) {
    state0 = rhs.state0;
    state1 = rhs.state1;
    state2 = rhs.state2;
    return *this;
  }
  word32 state0;
  word32 state1;
  word32 state2;
};

cipher_state cipher_update_alpha(cipher_state cs, word8 byte)
{
  cipher_state ns = cs;
  ns.state0 = crc32(ns.state0, byte);
  return ns;
}

cipher_state cipher_update_beta(cipher_state cs)
{
  cipher_state ns = cs;
  ns.state1 = ns.state1 + (ns.state0 & word32(0xff));
  ns.state1 = ns.state1 * word32(134775813) + word32(1);
  return ns;
}

cipher_state cipher_update_gamma(cipher_state cs)
{
  cipher_state ns = cs;
  ns.state2 = crc32(ns.state2, word8(ns.state1 >> 24));
  return ns;
}

cipher_state cipher_update(cipher_state cs, word8 byte)
{
  cipher_state ns = cipher_update_alpha(cs, byte);
  ns = cipher_update_beta(ns);
  ns = cipher_update_gamma(ns);
  return ns;
}

word8 cipher_keystream(cipher_state cs)
{
  word16 temp = word16(cs.state2 | word32(2));
  return word8((temp * (temp ^ word16(1))) >> 8);
}

word8 cipher_encrypt(cipher_state& cs, word8 byte)
{
  word8 out = cipher_keystream(cs) ^ byte;
  cs = cipher_update(cs, byte);
  return out;
}

word8 cipher_decrypt(cipher_state& cs, word8 byte)
{
  word8 out = cipher_keystream(cs) ^ byte;
  cs = cipher_update(cs, out);
  return out;
}

cipher_state cipher_init(const std::string& key)
{
  cipher_state cs;
  cs.state0 = word32(305419896);
  cs.state1 = word32(591751049);
  cs.state2 = word32(878082192);
  for (size_t i = 0; i < key.length(); ++i) {
    cs = cipher_update(cs, word8(key[i]));
  }
  return cs;
}

template <typename T> T named_word(const std::string& prefix, const size_t i, const size_t j) {
  std::stringstream ss;
  ss << prefix << i << "_" << j;
  return T(ss.str());
}

cipher_state cipher_init_unknown(size_t i)
{
  cipher_state cs;
  cs.state0 = named_word<word32>("state", i, 0);
  cs.state1 = named_word<word32>("state", i, 1);
  cs.state2 = named_word<word32>("state", i, 2);
  return cs;
}

struct cipher_bytes {
  std::vector<word8> bytes;
};

int main(int argc, char* argv[]) {

  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " <zip_file>" << std::endl;
    return 0;
  }

  solver s;

  std::vector<encrypted_file> files = load_file(argv[1]);

  std::vector<cipher_state> states;
  std::vector<cipher_bytes> ct;

  for (size_t i = 0; i < files.size(); ++i) {
    cipher_bytes ctb;
    for (size_t j = 0; j < files[i].bytes.size(); ++j) {
      ctb.bytes.push_back(word8(files[i].bytes[j]));
    }
    ct.push_back(ctb);
    states.push_back(cipher_init_unknown(i));
  }

  for (size_t i = 0; i < states.size(); ++i) {
    for (size_t j = i + 1; j < states.size(); ++j) {
      s.add(states[i].state0 == states[j].state0);
      s.add(states[i].state1 == states[j].state1);
      s.add(states[i].state2 == states[j].state2);
    }
  }

  for (size_t i = 0; i < states.size(); ++i) {
    for (size_t j = 0; j < 12; ++j) {
      assert(ct[i].bytes.size() > j);
      cipher_decrypt(states[i], ct[i].bytes[j]);
    }
  }

  for (size_t i = 12; i < 24; ++i) {
    std::vector<word8> pt;
    for (size_t j = 0; j < states.size(); ++j) {
      assert(ct[j].bytes.size() > i);
      word8 ptb = cipher_decrypt(states[j], ct[j].bytes[i]);
      pt.push_back(ptb);
    }
    for (size_t j = 0; j < pt.size(); ++j) {
      for (size_t k = j + 1; k < pt.size(); ++k) {
        s.add(pt[j] == pt[k]);
      }
    }
  }

  while (s.solve()) {
    s.print();
    s.next();
  }

  return 0;
}

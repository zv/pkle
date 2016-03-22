#include <gtest/gtest.h>
#include "solver.h"

TEST(s2, eval_add1) {
  solver ss;
  word4 x("x");
  word4 out = word4(1) + word4(3);
  word4 check = out == x;
  ss.add(check);
  EXPECT_TRUE(ss.solve());
  std::vector<solver::solution> r = ss.solutions();
  EXPECT_EQ(1, r.size());
  EXPECT_EQ(4, r[0].second);
}

TEST(s2, eval_add2) {
  solver ss;
  word4 x("x");
  word4 out = word4(3) + word4(3);
  word4 check = out == x;
  ss.add(check);
  EXPECT_TRUE(ss.solve());
  std::vector<solver::solution> r = ss.solutions();
  EXPECT_EQ(1, r.size());
  EXPECT_EQ(6, r[0].second);
}

TEST(s2, eval_add3) {
  solver ss;
  word4 x("x");
  word4 out = word4(7) + word4(7);
  word4 check = out == x;
  ss.add(check);
  EXPECT_TRUE(ss.solve());
  std::vector<solver::solution> r = ss.solutions();
  EXPECT_EQ(1, r.size());
  EXPECT_EQ(14, r[0].second);
}

TEST(s2, eval_mul1) {
  solver ss;
  word4 x("x");
  word4 out = word4(3) * word4(3);
  word4 check = out == x;
  ss.add(check);
  EXPECT_TRUE(ss.solve());
  std::vector<solver::solution> r = ss.solutions();
  EXPECT_EQ(1, r.size());
  EXPECT_EQ(9, r[0].second);
}

TEST(s2, eval_mul2) {
  solver ss;
  word4 x("x");
  word4 out = word4(2) * word4(3);
  word4 check = out == x;
  ss.add(check);
  EXPECT_TRUE(ss.solve());
  std::vector<solver::solution> r = ss.solutions();
  EXPECT_EQ(1, r.size());
  EXPECT_EQ(6, r[0].second);
}

TEST(s2, solve_add1) {
  solver ss;
  word4 x("x");
  word4 out = word4(3) + x;
  word4 check = out == word4(4);
  ss.add(check);
  EXPECT_TRUE(ss.solve());
  std::vector<solver::solution> r = ss.solutions();
  EXPECT_EQ(1, r.size());
  EXPECT_EQ(1, r[0].second);
}

TEST(s2, solve_add2) {
  solver ss;
  word4 x("x");
  word4 out = word4(3) + x;
  word4 check = out == word4(6);
  ss.add(check);
  EXPECT_TRUE(ss.solve());
  std::vector<solver::solution> r = ss.solutions();
  EXPECT_EQ(1, r.size());
  EXPECT_EQ(3, r[0].second);
}

TEST(s2, solve_add3) {
  solver ss;
  word4 x("x");
  word4 out = word4(1) + x;
  word4 check = out == word4(8);
  ss.add(check);
  EXPECT_TRUE(ss.solve());
  std::vector<solver::solution> r = ss.solutions();
  EXPECT_EQ(1, r.size());
  EXPECT_EQ(7, r[0].second);
}

TEST(s2, solve_mul1) {
  solver ss;
  word4 x("x");
  word4 out = word4(3) * x;
  word4 check = out == word4(9);
  ss.add(check);
  EXPECT_TRUE(ss.solve());
  std::vector<solver::solution> r = ss.solutions();
  EXPECT_EQ(1, r.size());
  EXPECT_EQ(3, r[0].second);
}

TEST(s2, solve_mul2) {
  solver ss;
  word8 x("x");
  word8 out = word8(6) * x;
  word8 check = out == word8(48);
  ss.add(check);
  EXPECT_TRUE(ss.solve());
  std::vector<solver::solution> r = ss.solutions();
  EXPECT_EQ(1, r.size());
  EXPECT_EQ(8, r[0].second);
}

TEST(s2, solve_double1) {
  solver ss;
  word8 x("x");
  word8 out = x + x;
  word8 check = out == word8(16);
  ss.add(check);
  EXPECT_TRUE(ss.solve());
  std::vector<solver::solution> r = ss.solutions();
  EXPECT_EQ(1, r.size());
  EXPECT_EQ(8, r[0].second);
}

TEST(s2, solve_double2) {
  solver ss;
  word8 x("x");
  word8 out = x + x;
  word8 check = out == word8(14);
  ss.add(check);
  EXPECT_TRUE(ss.solve());
  std::vector<solver::solution> r = ss.solutions();
  EXPECT_EQ(1, r.size());
  EXPECT_EQ(7, r[0].second);
}

TEST(s2, solve_double3) {
  solver ss;
  word8 x("x");
  word8 out = x + x;
  word8 check = out == word8(34);
  ss.add(check);
  EXPECT_TRUE(ss.solve());
  std::vector<solver::solution> r = ss.solutions();
  EXPECT_EQ(1, r.size());
  EXPECT_EQ(17, r[0].second);
}

TEST(s2, solve_square1) {
  solver ss;
  word8 x("x");
  word8 out = x * x;
  word8 check = out == word8(49);
  ss.add(check);
  EXPECT_TRUE(ss.solve());
  std::vector<solver::solution> r = ss.solutions();
  EXPECT_EQ(1, r.size());
  EXPECT_EQ(7, r[0].second);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

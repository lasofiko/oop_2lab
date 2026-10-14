#include "polynomial.h"
#include <gtest/gtest.h>
#include <stdexcept>
#include <utility>

TEST(Polynomial, Zero) {
    Polynomial p;
    EXPECT_EQ(p.degree(), -1);
    EXPECT_DOUBLE_EQ(p.evaluate(2), 0);
    EXPECT_DOUBLE_EQ(p.coefficient_sum(), 0);
}
TEST(Polynomial, Array) {
    double a[] = {1, 2, 3}; Polynomial p(a, 2);
    EXPECT_EQ(p.degree(), 2); EXPECT_DOUBLE_EQ(p.coefficient(2), 3);
    a[0] = 100; EXPECT_DOUBLE_EQ(p.coefficient(0), 1);
}
TEST(Polynomial, Constant) {
    double a[] = {7}; Polynomial p(a, 0);
    EXPECT_EQ(p.degree(), 0); EXPECT_DOUBLE_EQ(p.evaluate(5), 7);
}
TEST(Polynomial, CopyConstructor) {
    double a[] = {1, 2}; Polynomial p(a, 1); Polynomial q(p);
    EXPECT_EQ(q.degree(), 1); EXPECT_DOUBLE_EQ(q.evaluate(3), 7);
}
TEST(Polynomial, IndependentCopy) {
    double a[] = {1, 2}; Polynomial p(a, 1); Polynomial q(p);
    p = Polynomial(); EXPECT_DOUBLE_EQ(q.evaluate(3), 7);
}
TEST(Polynomial, MoveConstructor) {
    double a[] = {1, 2}; Polynomial p(a, 1); Polynomial q(std::move(p));
    EXPECT_DOUBLE_EQ(q.evaluate(3), 7); EXPECT_EQ(p.degree(), -1);
    EXPECT_DOUBLE_EQ(p.coefficient_sum(), 0);
    p = q; EXPECT_DOUBLE_EQ(p.evaluate(3), 7);
}
TEST(Polynomial, CopyAssignment) {
    double a[] = {1, 2}; double b[] = {9};
    Polynomial p(a, 1), q(b, 0); q = p; p = Polynomial();
    EXPECT_EQ(q.degree(), 1); EXPECT_DOUBLE_EQ(q.evaluate(3), 7);
}
TEST(Polynomial, MoveAssignment) {
    double a[] = {1, 2}; double b[] = {9};
    Polynomial p(a, 1), q(b, 0); q = std::move(p);
    EXPECT_DOUBLE_EQ(q.evaluate(3), 7); EXPECT_EQ(p.degree(), -1);
}
TEST(Polynomial, SelfAssignment) {
    double a[] = {1, 2}; Polynomial p(a, 1); p = p;
    EXPECT_DOUBLE_EQ(p.evaluate(3), 7);
    p = std::move(p); EXPECT_DOUBLE_EQ(p.evaluate(3), 7);
}
TEST(Polynomial, EmptyAssignment) {
    double a[] = {1, 2}; Polynomial p(a, 1), zero; p = zero;
    EXPECT_EQ(p.degree(), -1); EXPECT_DOUBLE_EQ(p.evaluate(3), 0);
}
TEST(Polynomial, TemporaryAssignment) {
    double a[] = {1, 2}; Polynomial p; p = Polynomial(a, 1);
    EXPECT_DOUBLE_EQ(p.evaluate(3), 7);
}
TEST(Polynomial, AddDifferentDegrees) {
    double a[] = {1, 2, 3}; double b[] = {-1, 4};
    Polynomial p(a, 2), q(b, 1); Polynomial r = p.add(q);
    EXPECT_EQ(r.degree(), 2); EXPECT_DOUBLE_EQ(r.coefficient(0), 0);
    EXPECT_DOUBLE_EQ(r.coefficient(1), 6); EXPECT_DOUBLE_EQ(r.coefficient(2), 3);
    EXPECT_DOUBLE_EQ(p.coefficient(1), 2);
}
TEST(Polynomial, Cancellation) {
    double a[] = {1, 2}; double b[] = {-1, -2};
    Polynomial r = Polynomial(a, 1).add(Polynomial(b, 1));
    EXPECT_EQ(r.degree(), -1); EXPECT_DOUBLE_EQ(r.coefficient_sum(), 0);
}
TEST(Polynomial, Multiply) {
    double a[] = {1, 2, 3}; double b[] = {-1, 1};
    Polynomial r = Polynomial(a, 2).mul(Polynomial(b, 1));
    EXPECT_EQ(r.degree(), 3);
    EXPECT_DOUBLE_EQ(r.coefficient(0), -1); EXPECT_DOUBLE_EQ(r.coefficient(1), -1);
    EXPECT_DOUBLE_EQ(r.coefficient(2), -1); EXPECT_DOUBLE_EQ(r.coefficient(3), 3);
}
TEST(Polynomial, ZeroArithmetic) {
    double a[] = {1, 2}; Polynomial p(a, 1), zero;
    EXPECT_EQ(p.mul(zero).degree(), -1);
    EXPECT_DOUBLE_EQ(zero.add(p).evaluate(3), 7);
    EXPECT_EQ(zero.add(zero).degree(), -1);
}
TEST(Polynomial, Evaluate) {
    double a[] = {1, 2, 3}; Polynomial p(a, 2);
    EXPECT_DOUBLE_EQ(p.evaluate(2), 17); EXPECT_DOUBLE_EQ(p.evaluate(-1), 2);
    EXPECT_DOUBLE_EQ(p.evaluate(0), 1);
}
TEST(Polynomial, Trim) {
    double a[] = {1, 2, 0, 0}; Polynomial p(a, 3); p.trim();
    EXPECT_EQ(p.degree(), 1); EXPECT_DOUBLE_EQ(p.coefficient(2), 0);
    double b[] = {-1, -2, 3}; double c[] = {2, 4, -3};
    Polynomial r = Polynomial(b, 2).add(Polynomial(c, 2));
    EXPECT_EQ(r.degree(), 1); EXPECT_DOUBLE_EQ(r.evaluate(2), 5);
}
TEST(Polynomial, AllZero) {
    double a[] = {0, 0, 0}; Polynomial p(a, 2); p.trim();
    EXPECT_EQ(p.degree(), -1); EXPECT_DOUBLE_EQ(p.evaluate(5), 0);
}
TEST(Polynomial, CoefficientSum) {
    double a[] = {1, 2, 3}; Polynomial p(a, 2);
    EXPECT_DOUBLE_EQ(p.coefficient_sum(), 6);
    double b[] = {-2, 0, 5, -1}; Polynomial q(b, 3);
    EXPECT_DOUBLE_EQ(q.coefficient_sum(), 2);
    EXPECT_DOUBLE_EQ(q.coefficient_sum(), q.evaluate(1));
    EXPECT_EQ(q.degree(), 3); EXPECT_DOUBLE_EQ(q.coefficient(0), -2);
}
TEST(Polynomial, Swap) {
    double a[] = {1, 2}; Polynomial p(a, 1), q; p.swap(q);
    EXPECT_EQ(p.degree(), -1); EXPECT_DOUBLE_EQ(q.evaluate(3), 7);
}
TEST(Polynomial, InvalidInput) {
    double a[] = {1};
    EXPECT_THROW(Polynomial(a, -2), std::invalid_argument);
    EXPECT_THROW(Polynomial(nullptr, 0), std::invalid_argument);
    EXPECT_EQ(Polynomial(nullptr, -1).degree(), -1);
    EXPECT_THROW(Polynomial().coefficient(-1), std::out_of_range);
}

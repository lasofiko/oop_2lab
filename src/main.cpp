#include "polynomial.h"
#include <iostream>

int main() {
    const double a[] = {1, 2, 3};
    const double b[] = {-1, 1};
    const Polynomial p(a, 2);
    const Polynomial q(b, 1);
    std::cout << "P(x) = ";
    p.print(std::cout);
    std::cout << "\nQ(x) = ";
    q.print(std::cout);
    std::cout << "\nP + Q = ";
    p.add(q).print(std::cout);
    std::cout << "\nP * Q = ";
    p.mul(q).print(std::cout);
    std::cout << "\nP(2) = " << p.evaluate(2)
              << "\nCoefficient sum = " << p.coefficient_sum() << '\n';
}

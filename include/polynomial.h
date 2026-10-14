#pragma once

#include <iosfwd>

class Polynomial {
public:
    Polynomial() noexcept;
    Polynomial(const double* coefficients, int degree);
    Polynomial(const Polynomial& other);
    Polynomial(Polynomial&& other) noexcept;
    ~Polynomial();

    Polynomial& operator=(Polynomial other) noexcept;
    void swap(Polynomial& other) noexcept;

    int degree() const noexcept;
    double coefficient(int power) const;
    Polynomial add(const Polynomial& other) const;
    Polynomial mul(const Polynomial& other) const;
    double evaluate(double x) const noexcept;
    double coefficient_sum() const noexcept;
    void print(std::ostream& out) const;
    void trim();

private:
    double* coeffs_;
    int degree_;
};

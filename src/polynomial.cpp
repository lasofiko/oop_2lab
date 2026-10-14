#include "polynomial.h"

#include <algorithm>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <utility>

Polynomial::Polynomial() noexcept : coeffs_(nullptr), degree_(-1) {}

Polynomial::Polynomial(const double* coefficients, int degree)
    : coeffs_(nullptr), degree_(-1) {
    if (degree < -1) {
        throw std::invalid_argument("Degree must be at least -1");
    }
    if (degree == -1) {
        return;
    }
    if (coefficients == nullptr) {
        throw std::invalid_argument("Null coefficient array");
    }
    // Нормализуем степень до выделения памяти.
    while (degree >= 0 && coefficients[degree] == 0.0) {
        --degree;
    }
    if (degree >= 0) {
        coeffs_ = new double[static_cast<std::size_t>(degree) + 1];
        degree_ = degree;
        for (int i = 0; i <= degree_; ++i) {
            coeffs_[i] = coefficients[i];
        }
    }
}

Polynomial::Polynomial(const Polynomial& other)
    : Polynomial(other.coeffs_, other.degree_) {}

Polynomial::Polynomial(Polynomial&& other) noexcept
    : coeffs_(other.coeffs_), degree_(other.degree_) {
    other.coeffs_ = nullptr;
    other.degree_ = -1;
}

Polynomial::~Polynomial() {
    delete[] coeffs_;
}

Polynomial& Polynomial::operator=(Polynomial other) noexcept {
    swap(other);
    return *this;
}

void Polynomial::swap(Polynomial& other) noexcept {
    std::swap(coeffs_, other.coeffs_);
    std::swap(degree_, other.degree_);
}

int Polynomial::degree() const noexcept {
    return degree_;
}

double Polynomial::coefficient(int power) const {
    if (power < 0) {
        throw std::out_of_range("Negative power");
    }
    return power <= degree_ ? coeffs_[power] : 0.0;
}

void Polynomial::trim() {
    int new_degree = degree_;
    while (new_degree >= 0 && coeffs_[new_degree] == 0.0) {
        --new_degree;
    }
    if (new_degree == degree_) {
        return;
    }
    // Временный объект обеспечивает сохранность исходного при ошибке new[].
    Polynomial normalized(coeffs_, new_degree);
    swap(normalized);
}

Polynomial Polynomial::add(const Polynomial& other) const {
    Polynomial result;
    result.degree_ = std::max(degree_, other.degree_);
    if (result.degree_ >= 0) {
        result.coeffs_ = new double[static_cast<std::size_t>(result.degree_) + 1];
        for (int i = 0; i <= result.degree_; ++i) {
            result.coeffs_[i] = coefficient(i) + other.coefficient(i);
        }
        result.trim();
    }
    return result;
}

Polynomial Polynomial::mul(const Polynomial& other) const {
    Polynomial result;
    if (degree_ == -1 || other.degree_ == -1) {
        return result;
    }
    if (degree_ > std::numeric_limits<int>::max() - other.degree_) {
        throw std::overflow_error("Polynomial degree overflow");
    }
    result.degree_ = degree_ + other.degree_;
    result.coeffs_ = new double[static_cast<std::size_t>(result.degree_) + 1]{};
    for (int i = 0; i <= degree_; ++i) {
        for (int j = 0; j <= other.degree_; ++j) {
            result.coeffs_[i + j] += coeffs_[i] * other.coeffs_[j];
        }
    }
    result.trim();
    return result;
}

double Polynomial::evaluate(double x) const noexcept {
    double result = 0.0;
    // Схема Горнера: не нужно отдельно вычислять степени x.
    for (int i = degree_; i >= 0; --i) {
        result = result * x + coeffs_[i];
    }
    return result;
}

double Polynomial::coefficient_sum() const noexcept {
    double sum = 0.0;
    for (int i = 0; i <= degree_; ++i) {
        sum += coeffs_[i];
    }
    return sum;
}

void Polynomial::print(std::ostream& out) const {
    if (degree_ == -1) {
        out << '0';
        return;
    }
    bool first = true;
    for (int i = degree_; i >= 0; --i) {
        const double value = coeffs_[i];
        if (value == 0.0) {
            continue;
        }
        if (!first) {
            out << (value < 0.0 ? " - " : " + ");
        } else if (value < 0.0) {
            out << '-';
        }
        const double magnitude = value < 0.0 ? -value : value;
        if (i == 0 || magnitude != 1.0) {
            out << magnitude;
            if (i > 0) {
                out << '*';
            }
        }
        if (i > 0) {
            out << 'x';
            if (i > 1) {
                out << '^' << i;
            }
        }
        first = false;
    }
}

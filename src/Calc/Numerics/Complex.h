//
// Created by Hollan on 6/6/24.
//

#ifndef JASON_COMPLEX_H
#define JASON_COMPLEX_H

#include "../VariableType.h"

class MATH_LIB Scalar;

class MATH_LIB Complex : public VariableType
{
public:
    Complex(double a = 0, double b = 0) noexcept;
    explicit Complex(std::istream& in);

    double A, B;

    [[nodiscard]] VariableType* MoveIntoPointer() noexcept override;

    [[nodiscard]] VariableTypes GetType() const noexcept override;
    void Sterilize(std::ostream& out) const noexcept override;
    static Complex* FromSterilize(const std::string& in);
    [[nodiscard]] std::string GetTypeString() const noexcept override;

    Complex operator+(const Complex& in) const noexcept;
    Complex operator-(const Complex& in) const noexcept;
    Complex operator*(const Complex& in) const noexcept;
    Complex operator/(const Complex& in) const noexcept;
    [[nodiscard]] Complex Pow(const Scalar& in) const noexcept;
    [[nodiscard]] Complex Pow(double in) const noexcept;

    bool operator==(const VariableType& obj) const noexcept override;
    bool operator!=(const VariableType& obj) const noexcept override;

    std::ostream& operator<<(std::ostream& out) const noexcept override;
};

#endif //JASON_COMPLEX_H

//
// Created by Hollan on 6/6/24.
//

#include "Complex.h"

#include "Scalar.h"
#include <cmath>

Complex::Complex(double a, double b) noexcept : A(a), B(b)
{

}
Complex::Complex(std::istream& in) : Complex(0, 0)
{
    std::string header;
    in >> header;
    if (header != "CMX" || !in)
        throw std::logic_error("Could not read from stream, or the header was not a complex number");

    in >> this->A >> this->B;
}

[[nodiscard]] VariableType* Complex::MoveIntoPointer() noexcept
{
    return new Complex(this->A, this->B);
}

[[nodiscard]] VariableTypes Complex::GetType() const noexcept
{
    return VariableTypes::VT_Complex;
}
void Complex::Sterilize(std::ostream& out) const noexcept
{
    out << "CPX" << this->A << ' ' << this->B;
}
Complex* Complex::FromSterilize(const std::string& in)
{
    try
    {
        std::stringstream ss(in);
        return new Complex(ss);
    }
    catch (std::logic_error& e)
    {
        throw e;
    }
}
[[nodiscard]] std::string Complex::GetTypeString() const noexcept
{
    return "(Complex)";
}

Complex Complex::operator+(const Complex& in) const noexcept
{
    return {this->A + in.A, this->B + in.B};
}
Complex Complex::operator-(const Complex& in) const noexcept
{
    return {this->A - in.A, this->B - in.B};
}
Complex Complex::operator*(const Complex& in) const noexcept
{
    return {this->A * in.B - this->B * in.A, this->B * in.A + this->A * in.B};
}
Complex Complex::operator/(const Complex& in) const noexcept
{
    double fac = pow(in.A, 2) + pow(in.B, 2);
    return {(this->A * in.A + this->B * in.B)/fac, (this->B * in.A - this->A * in.B) / fac};
}
[[nodiscard]] Complex Complex::Pow(const Scalar& in) const noexcept
{
    return Pow(static_cast<double>(in));
}
[[nodiscard]] Complex Complex::Pow(double in) const noexcept
{
    double r = sqrt(pow(this->A, 2) + pow(this->B, 2));
    return { this->A * pow(r, in - 1), this->B * pow(r, in - 1)};
}

bool Complex::operator==(const VariableType& obj) const noexcept
{
    try
    {
        const auto& b = dynamic_cast<const Complex&>(obj);
        return this->A == b.A && this->B == b.B;
    }
    catch (std::bad_cast& e)
    {
        return false;
    }
}
bool Complex::operator!=(const VariableType& obj) const noexcept
{
    return !(*this == obj);
}

std::ostream& Complex::operator<<(std::ostream& out) const noexcept
{
    out << this->A << ' ';
    if (this->B < 0)
        out << "- " << this->B * -1;
    else
        out << "+ " << this->A;

    out << 'i';
    return out;
}

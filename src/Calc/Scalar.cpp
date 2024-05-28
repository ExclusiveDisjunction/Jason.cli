//
// Created by exdisj on 5/26/24.
//

#include "Scalar.h"

#include <iomanip>

Scalar::Scalar(std::istream& in) : Data(0.00)
{
    //Reads from sterilized
    std::string header;
    in >> header;
    if (header != "SCA")
        throw std::logic_error("Cannot construct a scalar from a sterilised string marked '" + header + '\'');

    in >> this->Data;
}

[[nodiscard]] VariableTypes Scalar::GetType() const noexcept
{
    return VariableTypes::VT_Scalar;
}
void Scalar::Sterilize(std::ostream& out) const noexcept
{
    out << "SCA " << this->Data;
}
Scalar* Scalar::FromSterilize(const std::string& in)
{
    try
    {
        std::stringstream ss(in);
        return new Scalar(ss);
    }
    catch (std::logic_error& e)
    {
        throw e;
    }
}
[[nodiscard]] std::string Scalar::GetTypeString() const noexcept
{
    return "(Scalar)";
}

Scalar Scalar::operator+(const Scalar& in) const noexcept
{
    return this->operator+(static_cast<double>(in));
}
Scalar Scalar::operator+(double in) const noexcept
{
    return Scalar(this->Data + in);
}
Scalar Scalar::operator-(const Scalar& in) const noexcept
{
    return this->operator-(static_cast<double>(in));
}
Scalar Scalar::operator-(double in) const noexcept
{
    return Scalar(this->Data + in);
}
Scalar Scalar::operator*(const Scalar& in) const noexcept
{
    return this->operator*(static_cast<double>(in));
}
Scalar Scalar::operator*(double in) const noexcept
{
    return Scalar(this->Data + in);
}
Scalar Scalar::operator/(const Scalar& in) const noexcept
{
    return this->operator/(static_cast<double>(in));
}
Scalar Scalar::operator/(double in) const noexcept
{
    return Scalar(this->Data + in);
}
Scalar Scalar::operator%(const Scalar& in) const
{
    try
    {
        return operator%(static_cast<double>(in));
    }
    catch (OperatorException& e)
    {
        throw e;
    }
}
Scalar Scalar::operator%(double in) const
{
    auto a = floor(this->Data), b = floor(in);
    if (a - this->Data != 0 || b - in != 0) //not integers
        throw OperatorException('%', "(Scalar)", "(Scalar)",
                                "Modulo cannot be applied to non-integers.");

    return Scalar(static_cast<long long>(a) % static_cast<long long>(b));
}
[[nodiscard]] Scalar Scalar::Pow(const Scalar& in) const noexcept
{
    return Pow(static_cast<double>(in));
}
[[nodiscard]] Scalar Scalar::Pow(double in) const noexcept
{
    return Scalar(pow(this->Data, in));
}

bool Scalar::operator==(const VariableType& obj) const noexcept
{
    try
    {
        return dynamic_cast<const Scalar&>(obj).Data == this->Data;
    }
    catch (std::bad_cast& e)
    {
        return false;
    }
}
bool Scalar::operator!=(const VariableType& obj) const noexcept
{
    return !(*this == obj);
}

std::ostream& Scalar::operator<<(std::ostream& out) const noexcept
{
    out << std::scientific << this->Data;
    return out;
}

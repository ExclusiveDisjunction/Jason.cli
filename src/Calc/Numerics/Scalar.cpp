//
// Created by exdisj on 5/26/24.
//

#include "Scalar.h"

#include <iomanip>

template<typename T> requires IsScalarOrDouble<T>
Scalar::Scalar(const T& Item) : Data(static_cast<double>(Item))
{

}
Scalar::Scalar(std::istream& in)
{

}

[[nodiscard]] VariableTypes Scalar::GetType() const noexcept
{
    return VariableTypes::VT_Scalar;
}
void Scalar::Sterilize(std::ostream& out) const noexcept
{
    out << "SCA " << this->operator double();
}
Scalar* Scalar::FromSterilize(const std::string& in)
{
    try
    {
        std::stringstream ss(in);
        return FromSterilize(ss);
    }
    catch (std::logic_error& e)
    {
        throw e;
    }
}
Scalar* Scalar::FromSterilize(std::istream& in)
{
    std::string header;
    std::streampos pos = in.tellg();
    in >> header;
    if (header != "SCA")
        throw std::logic_error("Cannot construct a scalar from this object.");

    in.seekg(pos);
    return new Scalar(in);
}
[[nodiscard]] std::string Scalar::GetTypeString() const noexcept
{
    return "(Scalar)";
}

[[nodiscard]] VariableType* Scalar::MoveIntoPointer() noexcept
{
    auto* result = new Scalar(*this);
    this->Data = 0;
    return result;
}

template<typename T> requires IsScalarOrDouble<T>
Scalar Scalar::operator+(const T& in) const noexcept
{
    Scalar result(*this);
    result += in;
    return result;
}
template<typename T> requires IsScalarOrDouble<T>
Scalar Scalar::operator-(const T& in) const noexcept
{
    Scalar result(*this);
    result -= in;
    return result;
}
template<typename T> requires IsScalarOrDouble<T>
Scalar Scalar::operator*(const T& in) const noexcept
{
    Scalar result(*this);
    result *= in;
    return result;
}
template<typename T> requires IsScalarOrDouble<T>
Scalar Scalar::operator/(const T& in) const
{
    //FIX THIS LATER
    auto conv = static_cast<double>(in);
    if (conv == 0) //div by zero
        throw OperatorException('/', this->GetTypeString(), this->GetTypeString(), "Divide by zero");

    Scalar result(*this);
    result /= conv;
    return result;
}

template<typename T> requires IsScalarOrDouble<T>
Scalar& Scalar::operator+=(const T& in) noexcept
{
    this->Data += static_cast<double>(in);
    return *this;
}
template<typename T> requires IsScalarOrDouble<T>
Scalar& Scalar::operator-=(const T& in) noexcept
{
    this->Data += static_cast<double>(in);
    return *this;
}
template<typename T> requires IsScalarOrDouble<T>
Scalar& Scalar::operator*=(const T& in) noexcept
{
    this->Data += static_cast<double>(in);
    return *this;
}
template<typename T> requires IsScalarOrDouble<T>
Scalar& Scalar::operator/=(const T& in)
{
    auto conv = static_cast<double>(in);
    if (conv == 0)
        throw OperatorException('/', this->GetTypeString(), this->GetTypeString(), "Divide by zero");

    this->Data /= conv;
    return *this;
}

template<typename T> requires IsScalarOrDouble<T>
[[nodiscard]] Scalar Scalar::Pow(const T& in) const noexcept
{
    return Scalar { pow(static_cast<double>(*this), static_cast<double>(in)) };
}

[[nodiscard]] long long Scalar::ToLongNoRound() const
{
    long long Trunc = ToLongTrunc();
    double diff = static_cast<double>(Trunc) - this->Data;
    diff = diff < 0 ? -1 * diff : diff;

    if (diff > 5E-6)
        throw std::logic_error("The instance cannot be converted to a long long.");
    else
        return Trunc;
}
[[nodiscard]] long long Scalar::ToLongTrunc() const noexcept
{
    return static_cast<long long>(this->Data);
}

constexpr Scalar::operator double() const noexcept
{
    return this->Data;
}

bool Scalar::operator==(const VariableType& obj) const noexcept
{
    try
    {
        return dynamic_cast<const Scalar&>(obj).operator double() == this->operator double();
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

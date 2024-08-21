//
// Created by exdisj on 5/26/24.
//

#include "include/Numerics/Scalar.h"

#include <iomanip>

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
    std::stringstream ss(in);
    return FromSterilize(ss);
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

Scalar::operator double() const noexcept
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
bool Scalar::operator==(double obj) const noexcept
{
    return this->Data == obj;
}
bool Scalar::operator!=(double obj) const noexcept
{
    return this->Data != obj;
}

void Scalar::Print(std::ostream& out) const noexcept
{
    out << this->Data;
}

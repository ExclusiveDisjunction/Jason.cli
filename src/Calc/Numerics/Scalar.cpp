//
// Created by exdisj on 5/26/24.
//

#include "Scalar.h"

#include <iomanip>

VariableTypes Scalar::GetType() const noexcept
{
    return VariableTypes::VT_Scalar;
}
void Scalar::Sterilize(std::ostream& out) const noexcept
{
    out << "SCA " << this->operator double();
}
Scalar Scalar::Desterilize(std::istream& in)
{
    std::string header;
    std::streampos pos = in.tellg();
    in >> header;
    if (header != "SCA")
        throw std::logic_error("Cannot construct a scalar from this stream");

    Scalar result;
    in >> result.Data;
    return result;
}
std::unique_ptr<Scalar> Scalar::DesterilizePtr(std::istream& in)
{
    return std::make_unique<Scalar>(std::move(Desterilize(in)));
}
std::string Scalar::GetTypeString() const noexcept
{
    return "(Scalar)";
}

std::unique_ptr<VariableType> Scalar::Clone() const noexcept
{
    return std::make_unique<Scalar>(*this);
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
std::istream& operator>>(std::istream& in, Scalar& obj)
{
    obj = Scalar::Desterilize(in);
    return in;
}

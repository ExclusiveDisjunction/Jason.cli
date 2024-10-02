#include "MathVector.h"

//Includes templated functions for MathVector.

template<std::convertible_to<double>... Args>
MathVector MathVector::FromList(Args... Value) noexcept
{
    auto ToFill = std::vector<double>({((double) Value)...});
    size_t Dim = ToFill.size();
    MathVector result(Dim, 0);

    if (Dim != 0)
    {
        for (size_t i = 0; i < Dim; i++)
            result[i] = ToFill[i];
    }

    return result;
}

template<typename T> requires IsScalarOrDouble<T>
MathVector MathVector::operator*(const T& in) const
{
    MathVector result(*this);
    result.operator*=<T>(in);
    return result;
}
template<typename T> requires IsScalarOrDouble<T>
MathVector MathVector::operator/(const T& in) const
{
    MathVector result(*this);
    result.operator/=<T>(in);
    return result;
}

template<typename T> requires IsScalarOrDouble<T>
MathVector& MathVector::operator*=(const T& in)
{
    if (!this->IsValid())
        throw OperatorException('*', this->GetTypeString(), "(Scalar)", "Cannot multiply an error vector.");

    auto fac = static_cast<double>(in);
    for (unsigned i = 0; i < this->d && this->Point; i++)
        this->Point[i] *= fac;

    return *this;
}
template<typename T> requires IsScalarOrDouble<T>
MathVector& MathVector::operator/=(const T& in)
{
    if (!this->IsValid())
        throw OperatorException('*', this->GetTypeString(), "(Scalar)", "Cannot divide an error vector.");

    auto fac = static_cast<double>(in);
    for (unsigned i = 0; i < this->d && this->Point; i++)
        this->Point[i] /= fac;

    return *this;
}
#include "MathVector.h"

//Includes templated functions for MathVector.

template<std::convertible_to<double>... Args>
MathVector::MathVector(Args... Value) noexcept : MathVector()
{
    auto ToFill = std::vector<double>({((double) Value)...});
    size_t Dim = ToFill.size();

    if (Dim == 0)
        DeAllocate();
    else
    {
        Allocate(Dim, 0);

        for (size_t i = 0; i < Dim; i++)
            Point[i] = ToFill[i];
    }
}

template<typename T> requires IsScalarOrDouble<T>
MathVector MathVector::operator*(const T& in) const
{
    try
    {
        MathVector result(*this);
        result *= in;
        return result;
    }
    catch (OperatorException& e)
    {
        throw e;
    }
}
template<typename T> requires IsScalarOrDouble<T>
MathVector MathVector::operator/(const T& in) const
{
    try
    {
        MathVector result(*this);
        result /= in;
        return result;
    }
    catch (OperatorException& e)
    {
        throw e;
    }
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
}
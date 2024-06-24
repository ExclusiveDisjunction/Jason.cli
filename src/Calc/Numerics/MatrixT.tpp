#include "Matrix.h"

//Includes templated functions for matrix.

template<typename T> requires IsScalarOrDouble<T>
Matrix Matrix::operator*(const T& Two) const
{
    Matrix result(*this);
    result.operator*=<T>(Two);
    return result;
}
template<typename T> requires IsScalarOrDouble<T>
Matrix Matrix::operator/(const T& Two) const
{
    Matrix result(*this);
    result.operator/=<T>(Two);
    return result;
}

template<typename T> requires IsScalarOrDouble<T>
Matrix& Matrix::operator*=(const T& Two)
{
    if (!this->IsValid())
        throw OperatorException('*', this->GetTypeString(), "(Scalar)", "Empty Matrix");

    auto fac = static_cast<double>(Two);
    for (unsigned i = 0; i < this->m && this->Data; i++)
        for (unsigned j = 0; j < this->n; j++)
            this->Data[i][j] *= fac;

    return *this;
}
template<typename T> requires IsScalarOrDouble<T>
Matrix& Matrix::operator/=(const T& Two)
{
    if (!this->IsValid())
        throw OperatorException('/', this->GetTypeString(), "(Scalar)", "Empty Matrix");

    auto fac = static_cast<double>(Two);
    for (unsigned i = 0; i < this->m && this->Data; i++)
        for (unsigned j = 0; j < this->n; j++)
            this->Data[i][j] /= fac;

    return *this;
}
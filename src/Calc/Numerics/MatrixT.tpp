#include "Matrix.h"

//Includes templated functions for matrix.

template<std::convertible_to<double>... args>
[[maybe_unused]] Matrix::Matrix(unsigned Rows, unsigned Columns, args... vals) : Matrix(Rows, Columns)
{
    std::vector<double> conv = std::vector<double>({ static_cast<double>(vals)... });
    if (conv.size() != Rows * Columns)
        throw std::logic_error("Not enough or too many numbers provided.");

    unsigned k = 0;
    for (unsigned i = 0; i < Rows; i++, k++)
        for (unsigned j = 0; j < Columns; j++, k++)
            this->Data[i][j] = conv[k];
}

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
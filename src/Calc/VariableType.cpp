//
// Created by exdisj on 5/26/24.
//

#include "VariableType.h"

#include "Scalar.h"
#include "MathVector.h"
#include "Matrix.h"

std::string VariableType::Sterilize() const noexcept
{
    std::stringstream ss;
    Sterilize(ss);
    return ss.str();
}

MATH_LIB VariableType* FromSterilized(const std::string& val) noexcept
{
    std::stringstream ss(val);
    return FromSterilized(ss);
}
MATH_LIB VariableType* FromSterilized(std::istream& in) noexcept
{
    std::string header;
    in >> header;
    in.seekg(std::ios::beg);

    if (header == "NAN")
        return nullptr;
    else if (header == "SCA")
        return new Scalar(in);
    else if (header == "VEC")
        return new MathVector(in);
    else if (header == "MAT")
        return new Matrix(in);
}

MATH_LIB VariableType* ApplyOperation(const VariableType* one, const VariableType* two, char oper) noexcept
{
    if (!one || !two)
        return nullptr; //Cannot apply any operations on none.

    switch (oper)
    {
        case '+':
            return one->operator+(*two);
        case '-':
            return one->operator-(*two);
        case '*':
            return one->operator*(*two);
        case '/':
            return one->operator/(*two);
        case '%':
            return one->operator%(*two);
        case '^':
            return one->Pow(*two);
        default:
            return nullptr;
    }
}
MATH_LIB VariableType* AddVar(const VariableType* one, const VariableType* two) noexcept
{
    return ApplyOperation(one, two, '+');
}
MATH_LIB VariableType* SubVar(const VariableType* one, const VariableType* two) noexcept
{
    return ApplyOperation(one, two, '-');
}
MATH_LIB VariableType* MulVar(const VariableType* one, const VariableType* two) noexcept
{
    return ApplyOperation(one, two, '*');
}
MATH_LIB VariableType* DivVar(const VariableType* one, const VariableType* two) noexcept
{
    return ApplyOperation(one, two, '/');
}
MATH_LIB VariableType* ModVar(const VariableType* one, const VariableType* two) noexcept
{
    return ApplyOperation(one, two, '%');
}
MATH_LIB VariableType* PowVar(const VariableType* one, const VariableType* two) noexcept
{
    return ApplyOperation(one, two, '^');
}

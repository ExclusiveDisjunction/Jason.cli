//
// Created by exdisj on 5/26/24.
//

#include "VariableType.h"

#include "Scalar.h"
#include "MathVector.h"
#include "Matrix.h"

#include "OperatorException.h"

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

[[nodiscard]] VariableType *VariableType::ApplyOperation(const VariableType* One, const VariableType* Two, char oper)
{
    if (!One || !Two)
        return nullptr; //Cannot apply any operations on none.

    try
    {
        if (One->GetType() == VT_Scalar && Two->GetType() == VT_Scalar) //Trivial Case
        {
            const auto& A = dynamic_cast<const Scalar&>(*One);
            const auto& B = dynamic_cast<const Scalar&>(*Two);

            switch (oper)
            {
                case '+':
                    return new Scalar(A + B);
                case '-':
                    return new Scalar(A - B);
                case '*':
                    return new Scalar(A * B);
                case '/':
                    return new Scalar(A / B);
                case '%':
                {
                    //Might throw
                    try
                    {
                        return new Scalar(A % B);
                    }
                    catch (OperatorException& e)
                    {
                        throw e;
                    }
                }
                case '^':
                    return new Scalar(A.Pow(B));
                default:
                    return nullptr;
            }
        }
        else if (One->GetType() == VT_Vector && Two->GetType() == VT_Vector)
        {
            const auto& A = dynamic_cast<const MathVector&>(*One);
            const auto& B = dynamic_cast<const MathVector&>(*Two);

            try
            {
                switch (oper)
                {
                    case '+':
                        return new MathVector(A + B);
                    case '-':
                        return new MathVector(A - B);
                    case '*':
                    case '/':
                    case '%':
                    case '^':
                        throw OperatorException(oper, A.GetTypeString(), B.GetTypeString(), "Operation does not exist.");
                    default:
                        throw OperatorException(oper, false);
                }
            }
            catch (OperatorException& e)
            {
                throw e;
            }
        }
        else if (One->GetType() == VT_Matrix && Two->GetType() == VT_Matrix)
        {
            const auto& A = dynamic_cast<const Matrix&>(*One);
            const auto& B = dynamic_cast<const Matrix&>(*Two);

            try
            {
                switch (oper)
                {
                    case '+':
                        return new Matrix(A + B);
                    case '-':
                        return new Matrix(A - B);
                    case '*':
                        return new Matrix(A * B);
                    case '/':
                    case '%':
                    case '^':
                        throw OperatorException(oper, A.GetTypeString(), B.GetTypeString(), "Operation does not exist.");
                    default:
                        throw OperatorException(oper, false);
                }
            }
            catch (OperatorException& e)
            {
                throw e;
            }
        }

        /*
         * Now that the trival cases are out of the way, there are the more exotic cross types.
         * These are:
         *  1. Scalar * Vector
         *  2. Vector * Scalar
         *  3. Scalar * Matrix
         *  4. Matrix * Scalar
         *  5. Matrix * Vector (Given d == n)
         *  6. Matrix ^ Scalar (Given (Sca -> N || Sca == 0)  && Matrix is square if Sca >= 2)
         *  7. Matrix + Vector (Given dims match)
         *  8. Vector + Matrix (Given dims match)
        */
    }
    catch (std::bad_cast& e)
    {
        throw std::logic_error("A bad cast exception was thrown, meaning that a type lied about its GetType.");
    }
}

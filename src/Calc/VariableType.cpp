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
        VariableTypes t1 = One->GetType(), t2 = Two->GetType();

        if (t1 == VT_Scalar && t2 == VT_Scalar) //Trivial Case
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
                    return new Scalar(A % B);
                case '^':
                    return new Scalar(A.Pow(B));
                default:
                    return nullptr;
            }
        }
        else if (t1 == VT_Vector && t2 == VT_Vector)
        {
            const auto& A = dynamic_cast<const MathVector&>(*One);
            const auto& B = dynamic_cast<const MathVector&>(*Two);

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
        else if (t1 == VT_Matrix && t2 == VT_Matrix)
        {
            const auto& A = dynamic_cast<const Matrix&>(*One);
            const auto& B = dynamic_cast<const Matrix&>(*Two);

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

        /*
         * Now that the trivial cases are out of the way, there are the more exotic cross types.
         * These are:
         *  1. Scalar * Vector
         *  2. Vector * Scalar
         *  3. Scalar * Matrix
         *  4. Matrix * Scalar
         *  5. Matrix * Vector (Given d == n)
         *  6. Matrix ^ Scalar (Given (Sca -> N || Sca == 0)  && Matrix is square if Sca >= 2)
         *  7. Matrix + Vector (Given dims match)
         *  8. Vector + Matrix (Given dims match)
         *  9. Matrix - Vector (Given dims match)
         *  10. Vector - Matrix (Given dims match)
        */
        switch (oper)
        {
            case '+':
            {
                if (t1 == VT_Matrix && t2 == VT_Vector)
                    return new Matrix(dynamic_cast<const Matrix&>(*One) + static_cast<const Matrix&>(dynamic_cast<const MathVector&>(*Two)));
                else if (t1 == VT_Vector && t2 == VT_Matrix)
                    return new Matrix(dynamic_cast<const Matrix&>(*Two) + static_cast<const Matrix&>(dynamic_cast<const MathVector&>(*One)));
                else
                    throw OperatorException(oper, One->GetTypeString(), Two->GetTypeString());
            }
            case '-':
            {
                if (t1 == VT_Matrix && t2 == VT_Vector)
                    return new Matrix(dynamic_cast<const Matrix&>(*One) - static_cast<const Matrix&>(dynamic_cast<const MathVector&>(*Two)));
                else if (t1 == VT_Vector && t2 == VT_Matrix)
                    return new Matrix(dynamic_cast<const Matrix&>(*Two) - static_cast<const Matrix&>(dynamic_cast<const MathVector&>(*One)));
                else
                    throw OperatorException(oper, One->GetTypeString(), Two->GetTypeString());
            }
            case '*':
            {
                if (t1 == VT_Scalar && t2 == VT_Vector)
                    return new MathVector(dynamic_cast<const MathVector&>(*Two) * dynamic_cast<const Scalar&>(*One));
                else if (t1 == VT_Vector && t2 == VT_Scalar)
                    return new MathVector(dynamic_cast<const MathVector&>(*One) * dynamic_cast<const Scalar&>(*Two));
                else if (t1 == VT_Scalar && t2 == VT_Matrix)
                    return new Matrix(dynamic_cast<const Matrix&>(*Two) * dynamic_cast<const Scalar&>(*One));
                else if (t1 == VT_Matrix && t2 == VT_Scalar)
                    return new Matrix(dynamic_cast<const Matrix&>(*One) * dynamic_cast<const Scalar&>(*Two));
                else if (t1 == VT_Matrix && t2 == VT_Vector)
                    return new Matrix(dynamic_cast<const Matrix&>(*One) *
                                      static_cast<const Matrix&>(dynamic_cast<const MathVector&>(*Two)));

                throw OperatorException(oper, One->GetTypeString(), Two->GetTypeString());
            }
            case '^':
            {
                if (t1 == VT_Matrix && t2 == VT_Scalar)
                {
                    const auto& A = dynamic_cast<const Matrix&>(*One);
                    const auto& B = dynamic_cast<const Scalar&>(*Two);

                    return new Matrix(A.Pow(B));
                }

                throw OperatorException(oper, One->GetTypeString(), Two->GetTypeString());
            }
            default:
                throw OperatorException(oper, One->GetTypeString(), Two->GetTypeString());
        }

    }
    catch (std::bad_cast& e)
    {
        throw std::logic_error("A bad cast exception was thrown, meaning that a type lied about its GetType.");
    }
    catch (OperatorException& e)
    {
        throw e;
    }
}

#include "../CoreFunctions.h"

Monomial::Monomial(unsigned int InputDim, unsigned int Var, double A, double N) : FunctionBase(InputDim, 1)
{
    this->N = N;
    this->VarLetter = Var;
    this->A = A;
}

MathVector Monomial::Evaluate(const MathVector& X, bool& Exists) const noexcept
{
    if (X.Dim() != InputDim)
    {
        Exists = false;
        return MathVector::ErrorVector();
    }

    return MathVector(A * pow(X[VarLetter], N));
}

[[nodiscard]] bool Monomial::ComparesTo(const FunctionBase* Obj) const noexcept
{
    const auto* conv = dynamic_cast<const Monomial*>(Obj);
    return conv != nullptr && conv->VarLetter == this->VarLetter && conv->N == this->N;
}
[[nodiscard]] bool Monomial::EquatesTo(const FunctionBase* Obj) const noexcept
{
    const auto* conv = dynamic_cast<const Monomial*>(Obj);
    return conv != nullptr && conv->VarLetter == this->VarLetter && conv->N == this->N && conv->A == this->A;
}
FunctionBase* Monomial::Clone() const noexcept
{
    return new Monomial(InputDim, VarLetter, A, N);
}
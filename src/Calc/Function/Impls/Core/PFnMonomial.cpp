#include "../CoreFunctions.h"

PFnMonomial::PFnMonomial(unsigned int InputDim, FunctionBase* B, FunctionBase* N, double A) : FunctionBase(InputDim, 1)
{
    if (!B || !N || B->InputDim != InputDim || N->InputDim != InputDim || B->OutputDim != 1 || N->OutputDim != 1)
        throw std::logic_error("The functions provided must have the following criteria: 1) Exist, 2) Their input dimension must match the provided input dimension, 3) Their output dimension must be one. One or more of these critera are not met.");

    Base(B);
    Power(N);
    this->A = A;
}

void PFnMonomial::ChildRemoved(FunctionBase* Obj) noexcept
{
    if (Obj == B)
        B = nullptr;
    else if (Obj == N)
        N = nullptr;
}

const FunctionBase& PFnMonomial::Base() const
{
    return Get(B);
}
FunctionBase& PFnMonomial::Base()
{
    return Get(B);
}
const FunctionBase& PFnMonomial::Power() const
{
    return Get(N);
}
FunctionBase& PFnMonomial::Power()
{
    return Get(N);
}

void PFnMonomial::Base(FunctionBase* New)
{
    PushAndBind(B, New);
}
void PFnMonomial::Power(FunctionBase* New)
{
    PushAndBind(N, New);
}

[[nodiscard]] bool PFnMonomial::ComparesTo(const FunctionBase* Obj) const noexcept
{
    const auto* conv = dynamic_cast<const PFnMonomial*>(Obj);
    return conv == this || (conv && conv->N->EquatesTo(this->N) && conv->B->EquatesTo(this->B));
}
[[nodiscard]] bool PFnMonomial::EquatesTo(const FunctionBase* Obj) const noexcept
{
    const auto* conv = dynamic_cast<const PFnMonomial*>(Obj);
    return conv == this || (conv && conv->N->EquatesTo(this->N) && conv->B->EquatesTo(this->B) && conv->A == this->A);
}

MathVector PFnMonomial::Evaluate(const MathVector& X, bool& Exists) const noexcept
{
    if (X.Dim() != InputDim || !B || !N)
    {
        Exists = false;
        return MathVector::ErrorVector();
    }

    Exists = true;
    MathVector BaseV = B->Evaluate(X, Exists);
    if (!Exists)
        return MathVector::ErrorVector();

    MathVector PowerV = N->Evaluate(X, Exists);
    if (!Exists)
        return MathVector::ErrorVector();

    return MathVector(A * pow(BaseV[0], PowerV[0]));
}

FunctionBase* PFnMonomial::Clone() const noexcept
{
    if (!B || !N)
        return nullptr;

    try
    {
        return new PFnMonomial(InputDim, B->Clone(), N->Clone(), A);
    }
    catch (std::exception& e)
    {
        return  nullptr;
    }
}
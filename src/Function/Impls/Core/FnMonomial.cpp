#include "../CoreFunctions.h"

FnMonomial::FnMonomial(FunctionBase* InnerFunction, double Power, double A) : FunctionBase(!InnerFunction ? 0 : InnerFunction->InputDim, 1)
{
    if (!InnerFunction || InnerFunction->OutputDim != 1)
        throw std::logic_error("The inner function must be valid, and the output dimension must be one.");

    this->A = A;
    this->N = Power;

    Base(InnerFunction);
}

const FunctionBase& FnMonomial::Base() const
{
    return Get(B);
}
FunctionBase& FnMonomial::Base()
{
    return Get(B);
}
void FnMonomial::Base(FunctionBase* NewB)
{
    PushAndBind(B, NewB);
}

void FnMonomial::ChildRemoved(FunctionBase* Obj) noexcept
{
    if (B == Obj)
        B = nullptr;
}

MathVector FnMonomial::Evaluate(const MathVector& X, bool& Exists) const noexcept
{
    if (X.Dim() != InputDim || !B)
    {
        Exists = false;
        return MathVector::ErrorVector();
    }

    Exists = true;
    MathVector InnerEval = B->Evaluate(X, Exists);
    if (!Exists)
        return MathVector::ErrorVector();

    return MathVector(A * pow(InnerEval[0], N));
}

[[nodiscard]] bool FnMonomial::ComparesTo(const FunctionBase* Obj) const noexcept
{
    const auto* conv = dynamic_cast<const FnMonomial*>(Obj);
    return this == conv || (conv && conv->N == this->N && conv->B->EquatesTo(this->B));
}
[[nodiscard]] bool FnMonomial::EquatesTo(const FunctionBase* Obj) const noexcept
{
    const auto* conv = dynamic_cast<const FnMonomial*>(Obj);
    return this == conv || (conv && conv->N == this->N && conv->B->EquatesTo(this->B) && this->A == conv->A);
}
FunctionBase* FnMonomial::Clone() const noexcept
{
    if (!B)
        return nullptr;

    try
    {
        return new FnMonomial(B->Clone(), N, A);
    }
    catch (std::exception& e)
    {
        return  nullptr;
    }
}
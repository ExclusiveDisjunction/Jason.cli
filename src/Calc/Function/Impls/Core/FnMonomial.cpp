#include "../CoreFunctions.h"

FnMonomial::FnMonomial(FunctionBase* InnerFunction, double A, double N) : FunctionBase(!InnerFunction ? 0 : InnerFunction->InputDim, 1)
{
    if (!InnerFunction || InnerFunction->OutputDim != 1)
        throw std::logic_error("The inner function must be valid, and the output dimension must be one.");

    this->A = A;
    this->N = N;

    Base(InnerFunction);
}

const FunctionBase& FnMonomial::Base() const
{
    if (!B)
        throw std::logic_error("This instance does not contain a valid Base function.");
    return *B;
}
FunctionBase& FnMonomial::Base()
{
    if (!B)
        throw std::logic_error("This instance does not contain a valid Base function.");
    return *B;
}
void FnMonomial::Base(FunctionBase* NewB)
{
    if (!NewB || NewB->InputDim != InputDim || NewB->OutputDim != 1) //IGNORE CMAKE warning because if !NewB, then InputDim == 0. So if !NewB, then this whole thing will fail and go to the exception.
        throw std::logic_error("The inner function must exist, and the input dimension must be the input dimension of this function, as well as the output dimension of the function being 1.");

    delete B;
    if (!this->PushChild(NewB))
        throw std::logic_error("Could not append new base.");

    this->B = NewB;
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
    return this == conv || (conv && conv->N == this->N && conv->B->ComparesTo(this->B));
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

    return new FnMonomial(B->Clone(), A, N);
}
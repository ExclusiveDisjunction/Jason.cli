#include "../CoreFunctions.h"

Logarithm::Logarithm(FunctionBase* N, double Base, double A) : FunctionBase(!N ? 0 : N->InputDim, 1)
{
    this->A = A;
    this->Base = Base;
    Function(N);
}

void Logarithm::ChildRemoved(FunctionBase* Child) noexcept
{
    if (Child == N)
        N = nullptr;
}

const FunctionBase& Logarithm::Function() const
{
    return Get(N);
}
FunctionBase& Logarithm::Function()
{
    return Get(N);
}
void Logarithm::Function(FunctionBase* Obj)
{
    PushAndBind(N, Obj);
}

MathVector Logarithm::Evaluate(const MathVector& X, bool& Exists) const noexcept
{
    if (X.Dim() != InputDim || !N)
    {
        Exists = false;
        return MathVector::ErrorVector();
    }

    Exists = true;
    MathVector Result = N->Evaluate(X, Exists);
    Exists &= Result[0] > 0.0; //Ensures that the log function does not receive a zero or negative value.
    if (!Exists)
        return MathVector::ErrorVector();

    return MathVector(A * log(Result[0]) / log(Base));
}

bool Logarithm::ComparesTo(const FunctionBase* Obj) const noexcept
{
    const auto* conv = dynamic_cast<const Logarithm*>(Obj);
    return this == conv || (conv && conv->Base == this->Base && conv->N->EquatesTo(this->N));
}
bool Logarithm::EquatesTo(const FunctionBase* Obj) const noexcept
{
    const auto* conv = dynamic_cast<const Logarithm*>(Obj);
    return this == conv || (conv && conv->Base == this->Base && conv->N->EquatesTo(this->N) && conv->A == this->A);
}
FunctionBase* Logarithm::Clone() const noexcept
{
    if (!N)
        return nullptr;

    try
    {
        return new Logarithm(N->Clone(), Base, A);
    }
    catch (std::exception& e)
    {
        return nullptr;
    }
}
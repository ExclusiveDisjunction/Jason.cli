#include "../CoreFunctions.h"

Exponent::Exponent(FunctionBase* N, double A, double B) : FunctionBase(!N ? 0 : N->InputDim, 1)
{
    this->Base = B;
    this->A = A;
    Power(N);
}

void Exponent::ChildRemoved(FunctionBase* Obj) noexcept
{
    if (Obj == N)
        N = nullptr;
}

const FunctionBase& Exponent::Power() const
{
    return Get(N);
}
FunctionBase& Exponent::Power()
{
    return Get(N);
}
void Exponent::Power(FunctionBase* NewFunction)
{
    PushAndBind(N, NewFunction);
}

MathVector Exponent::Evaluate(const MathVector& X, bool& Exists) const noexcept
{
    if (!N || X.Dim() != InputDim)
    {
        Exists = false;
        return MathVector::ErrorVector();
    }

    Exists = true;
    MathVector Result = N->Evaluate(X, Exists);
    if (!Exists)
        return MathVector::ErrorVector();

    return MathVector(A * pow(Base, Result[0]));
}

bool Exponent::ComparesTo(const FunctionBase* Obj) const noexcept
{
    const auto* conv = dynamic_cast<const Exponent*>(Obj);
    return conv == this || (conv && conv->Base == this->Base && conv->N->EquatesTo(this->N));
}
bool Exponent::EquatesTo(const FunctionBase* Obj) const noexcept
{
    const auto* conv = dynamic_cast<const Exponent*>(Obj);
    return conv == this || (conv && conv->Base == this->Base && conv->N->EquatesTo(this->N) && conv->A == this->A);
}
FunctionBase* Exponent::Clone() const noexcept
{
    if (!N)
        return nullptr;

    try
    {
        return new Exponent(N->Clone(), A, Base);
    }
    catch (std::exception& e)
    {
        return nullptr;
    }
}
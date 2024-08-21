#include "../CoreFunctions.h"

AbsoluteValue::AbsoluteValue(FunctionBase* N, double A) : FunctionBase(!N ? 0 : N->InputDim, 1)
{
    this->A = A;
    Base(N);
}

void AbsoluteValue::ChildRemoved(FunctionBase* Child) noexcept
{
    if (Child == N)
        N = nullptr;
}

const FunctionBase& AbsoluteValue::Base() const
{
    return Get(N);
}
FunctionBase& AbsoluteValue::Base()
{
    return Get(N);
}
void AbsoluteValue::Base(FunctionBase* New)
{
    PushAndBind(N, New);
}

MathVector AbsoluteValue::Evaluate(const MathVector& X, bool& Exists) const noexcept
{
    if (!N || X.Dim() != InputDim)
    {
        Exists = false;
        return MathVector::ErrorVector();
    }

    Exists = true;
    MathVector Base = N->Evaluate(X, Exists);
    if (!Exists)
        return MathVector::ErrorVector();

    return MathVector(A * Base.Magnitude());
}

[[nodiscard]] bool AbsoluteValue::ComparesTo(const FunctionBase* Obj) const noexcept
{
    const auto* conv = dynamic_cast<const AbsoluteValue*>(Obj);
    return this == conv || (conv && conv->N->EquatesTo(this->N));
}
[[nodiscard]] bool AbsoluteValue::EquatesTo(const FunctionBase* Obj) const noexcept
{
    const auto* conv = dynamic_cast<const AbsoluteValue*>(Obj);
    return this == conv || (conv && conv->N->EquatesTo(this->N) && conv->A == this->A);
}
FunctionBase* AbsoluteValue::Clone() const noexcept
{
    if (!N)
        return nullptr;

    try
    {
        return new AbsoluteValue(N->Clone(), A);
    }
    catch (std::exception& e)
    {
        return nullptr;
    }
}
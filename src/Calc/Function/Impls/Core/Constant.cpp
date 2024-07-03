#include "../CoreFunctions.h"

Constant::Constant(unsigned int InputDim, double A) : FunctionBase(InputDim, 1)
{
    this->A = A;
}

void Constant::ChildRemoved(FunctionBase* Child) noexcept
{
    //Constants do not have children.
}

MathVector Constant::Evaluate(const MathVector& X, bool& Exists) const noexcept
{
    if (X.Dim() != InputDim)
    {
        Exists = false;
        return MathVector::ErrorVector();
    }

    Exists = true;
    return MathVector(A);
}

bool Constant::ComparesTo(const FunctionBase* Obj) const noexcept
{
    return dynamic_cast<const Constant*>(Obj) != nullptr;
}
bool Constant::EquatesTo(const FunctionBase* Obj) const noexcept
{
    const auto* b = dynamic_cast<const Constant*>(Obj);
    return b != nullptr && b->A == this->A;
}
FunctionBase* Constant::Clone() const noexcept
{
    return new Constant(this->InputDim, this->A);
}
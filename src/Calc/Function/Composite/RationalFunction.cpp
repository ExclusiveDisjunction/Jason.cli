#include "RationalFunction.h"

RationalFunction::RationalFunction(unsigned int InputDim) : FunctionBase(InputDim, 1)
{

}

void RationalFunction::MultiplyFunction(FunctionBase* Obj)
{
    if (!this->PushChild(Obj))
        throw std::logic_error("Cannot add function to this RationalFunction.");
}
void RationalFunction::DivideFunction(FunctionBase* Obj)
{
    MultiplyFunction(Obj);
    //TODO: ADD FLAGS TO FUNCTIONS
}

MathVector RationalFunction::Evaluate(const MathVector& X, bool& Exists) const noexcept
{
    Exists = true;
    if (this->ChildCount() == 0)
    {
        Exists = false;
        return MathVector::ErrorVector();
    }

    double Return = 1;

    //TODO: REQUIRES ITERATORS

    return MathVector(Return);
}

bool RationalFunction::ComparesTo(const FunctionBase* Obj) const noexcept
{
    const auto* conv = dynamic_cast<const RationalFunction*>(Obj);
    return conv == this || (conv && conv->InputDim <= this->InputDim && conv->OutputDim <= this->OutputDim);
}
bool RationalFunction::EquatesTo(const FunctionBase* Obj) const noexcept
{
    const auto* conv = dynamic_cast<const RationalFunction*>(Obj);
    if (!conv || conv->InputDim != this->InputDim || conv->OutputDim != this->OutputDim || conv->A != this->A)
        return false;

    //TODO: REQUIRES ITERATORS

    return false;
}
FunctionBase* RationalFunction::Clone() const noexcept
{
    if (this->ChildCount() == 0)
        return new RationalFunction(InputDim);

    //TODO: REQUIRES ITERATORS

    return nullptr;
}
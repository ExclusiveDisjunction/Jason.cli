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
    Obj->SetFlag(FF_Rat_Inv, true);
}

const FunctionBase& RationalFunction::operator[](unsigned i) const
{
    return GetChildAt(i);
}
FunctionBase& RationalFunction::operator[](unsigned i)
{
    return GetChildAt(i);
}

[[maybe_unused]] [[nodiscard]] bool RationalFunction::RemoveFunction(FunctionBase* Obj, bool Delete)
{
    return PopChild(Obj, Delete);
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

    try
    {
        auto end = this->LastChild();
        for (auto iter = this->FirstChild(); iter != end; iter++)
        {
            const FunctionBase& func = *iter;
            MathVector eval = func.Evaluate(X, Exists);
            if (!Exists)
                return MathVector::ErrorVector();

            if (func.FlagActive(FunctionFlags::FF_Rat_Inv))
                Return /= eval[0];
            else
                Return *= eval[0];
        }
    }
    catch (std::logic_error&)
    {
        Exists = false;
        return MathVector::ErrorVector();
    }

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

    //TODO: Requires matching.

    return false;
}
FunctionBase* RationalFunction::Clone() const noexcept
{
    auto* Return = new RationalFunction(this->InputDim);
    if (this->ChildCount())
        Return->CloneChildrenFrom(this, false);

    return Return;
}
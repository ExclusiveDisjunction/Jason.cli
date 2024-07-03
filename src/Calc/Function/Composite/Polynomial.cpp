#include "Polynomial.h"

Polynomial::Polynomial(unsigned int InputDim, unsigned int OutputDim) : FunctionBase(InputDim, OutputDim)
{

}

void Polynomial::AddFunction(FunctionBase* Obj)
{
    if (!Obj)
        throw std::logic_error("Cannot add a nullptr");

    auto* conv = dynamic_cast<Polynomial*>(Obj);
    if (conv)
    {
        this->StealChildrenFrom(conv, false);
        delete conv;
    }
    else if (!this->PushChild(Obj))
        throw std::logic_error("Could not add function to the polynomial.");
}
void Polynomial::SubtractFunction(FunctionBase* Obj)
{
    if (!Obj)
        throw std::logic_error("Cannot subtract a nullptr");

    auto* conv = dynamic_cast<Polynomial*>(Obj);
    if (conv)
    {
        auto end = conv->LastChild();
        for (auto curr = conv->FirstChild(); curr != end; curr++)
            curr->InvertFlag(FunctionFlags::FF_Poly_Neg);

        this->StealChildrenFrom(conv, false);

        delete conv;
    }
    else
    {
        AddFunction(Obj);
        Obj->SetFlag(FF_Poly_Neg, true);
    }
}

[[maybe_unused]] [[nodiscard]] bool Polynomial::RemoveFunction(FunctionBase* Obj, bool Delete)
{
    return PopChild(Obj, Delete);
}

const FunctionBase& Polynomial::operator[](unsigned i) const
{
    return GetChildAt(i);
}
FunctionBase& Polynomial::operator[](unsigned i)
{
    return GetChildAt(i);
}

MathVector Polynomial::Evaluate(const MathVector& Obj, bool& Exists) const noexcept
{
    Exists = true;
    if (this->ChildCount() == 0)
    {
        Exists = false;
        return MathVector::ErrorVector();
    }

    MathVector Output(OutputDim);

    try
    {
        auto end = this->LastChild();
        for (auto iter = this->FirstChild(); iter != end; iter++)
        {
            const FunctionBase& func = *iter;
            MathVector eval = func.Evaluate(Obj, Exists);
            if (!Exists)
                return MathVector::ErrorVector();

            if (func.FlagActive(FunctionFlags::FF_Poly_Neg))
                Output -= eval;
            else
                Output += eval;
        }
    }
    catch (std::logic_error&)
    {
        Exists = false;
        return MathVector::ErrorVector();
    }

    return Output;
}

bool Polynomial::ComparesTo(const FunctionBase* Obj) const noexcept
{
    const auto* conv = dynamic_cast<const Polynomial*>(Obj);
    return conv == this || (conv && conv->InputDim <= this->InputDim && conv->OutputDim <= this->OutputDim);
}
bool Polynomial::EquatesTo(const FunctionBase* Obj) const noexcept
{
    const auto* conv = dynamic_cast<const Polynomial*>(Obj);
    if (!conv || conv->InputDim != this->InputDim || conv->OutputDim != this->OutputDim || conv->A != this->A)
        return false;

    //TODO: Requires matching.

    return false;
}
FunctionBase* Polynomial::Clone() const noexcept
{
    auto* Return = new Polynomial(this->InputDim, this->OutputDim);
    if (this->ChildCount() != 0)
        Return->CloneChildrenFrom(this, false);

    return Return;
}
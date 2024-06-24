#include "Polynomial.h"

Polynomial::Polynomial(unsigned int InputDim, unsigned int OutputDim) : FunctionBase(InputDim, OutputDim)
{

}

void Polynomial::AddFunction(FunctionBase* Obj)
{
    if (!this->PushChild(Obj))
        throw std::logic_error("Could not add function to the polynomial.");
}
void Polynomial::SubtractFunction(FunctionBase* Obj)
{
    AddFunction(Obj);
    Obj->operator-();
}

[[maybe_unused]] [[nodiscard]] bool Polynomial::RemoveFunction(FunctionBase* Obj, bool Delete)
{
    return PopChild(Obj, Delete);
}

const FunctionBase& Polynomial::operator[](unsigned i) const
{
    //TODO: REQUIRES ITERATORS
}
FunctionBase& Polynomial::operator[](unsigned i)
{
    //TODO: REQUIRES ITERATORS
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

    //TODO: REQUIRES ITERATORS

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

    //TODO: REQUIRES ITERATORS

    return false;
}
FunctionBase* Polynomial::Clone() const noexcept
{
    if (this->ChildCount() == 0)
        return new Polynomial(this->InputDim, this->OutputDim);

    //TODO: REQUIRES ITERATORS

    return nullptr;
}
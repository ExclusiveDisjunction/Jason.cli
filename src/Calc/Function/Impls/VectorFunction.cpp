#include "VectorFunction.h"

VectorFunction::VectorFunction(unsigned int InputDim, unsigned int OutputDim) : FunctionBase(InputDim, OutputDim)
{
    Fill(OutputDim);
}

void VectorFunction::Fill(unsigned int Dim)
{
    Clear();

    Func = new FunctionBase*[Dim];

    for (unsigned int i = 0; i < Dim; i++)
        Func[i] = nullptr;
}
void VectorFunction::Clear()
{
    if (Func)
    {
        for (unsigned int i = 0; i < OutputDim; i++)
            delete Func[i];

        delete[] Func;
    }

    Func = nullptr;
}
void VectorFunction::ChildRemoved(FunctionBase* Item) noexcept
{
    if (!Func)
        return;

    for (unsigned i = 0; i < OutputDim; i++)
    {
        if (Func[i] == Item)
        {
            Func[i] = nullptr;
            return;
        }
    }
}

const FunctionBase& VectorFunction::operator[](unsigned i) const
{
    if (!Func)
        throw std::logic_error("No functions contained.");
    else if (i > OutputDim)
        throw std::logic_error("Out of bounds");

    return Get(Func[i]);
}
FunctionBase& VectorFunction::operator[](unsigned i)
{
    if (!Func)
        throw std::logic_error("No functions contained.");
    else if (i > OutputDim)
        throw std::logic_error("Out of bounds");

    return Get(Func[i]);
}
void VectorFunction::AssignFunction(unsigned int Index, FunctionBase* New)
{
    if (Index > OutputDim || !Func)
        throw std::logic_error("Out of bounds or this instance has no children functions.");

    delete Func[Index];

    if (New && !this->PushChild(New)) //If !New, PushChild will always return false. So we only care if PushChild fails, provided that New is not nullptr.
        throw std::logic_error("Cannot push child.");

    Func[Index] = New;
}

MathVector VectorFunction::Evaluate(const MathVector& In, bool& Exists) const noexcept
{
    if (!Func || In.Dim() != InputDim)
    {
        Exists = false;
        return MathVector::ErrorVector();
    }

    MathVector Return(OutputDim);
    Exists = true;
    for (unsigned int i = 0; i < OutputDim; i++)
    {
        if (!Func[i])
        {
            Exists = false;
            return MathVector::ErrorVector();
        }

        MathVector Result = Func[i]->Evaluate(In, Exists);
        if (!Exists)
            return MathVector::ErrorVector();
        else
            Return[i] = Result[0];
    }

    return Return;
}

bool VectorFunction::ComparesTo(const FunctionBase* Obj) const noexcept
{
    const auto* conv = dynamic_cast<const VectorFunction*>(Obj);
    return conv == this || (conv && conv->InputDim == this->InputDim && conv->OutputDim == this->OutputDim); //Vector functions can always be added, if the dimensions match.
}
bool VectorFunction::EquatesTo(const FunctionBase* Obj) const noexcept
{
    const auto* conv = dynamic_cast<const VectorFunction*>(Obj);
    if (conv == this)
        return true;

    if (!conv || conv->InputDim != this->InputDim || conv->OutputDim != this->OutputDim || conv->A != this->A)
        return false;

    bool result = true;
    for (unsigned i = 0; i < this->OutputDim; i++)
        result &= this->Func[i] && conv->Func[i] && this->Func[i]->EquatesTo(conv->Func[i]);

    return result;
}
FunctionBase* VectorFunction::Clone() const noexcept
{
    auto* Return = new VectorFunction(InputDim, OutputDim);
    for (unsigned int i = 0; i < OutputDim; i++)
        Return->Func[i] = Func[i]->Clone();

    return Return;
}

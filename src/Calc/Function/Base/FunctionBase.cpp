#include "FunctionBase.h"

#include "../Composite/CompositeFunction.h"
#include "../Impls/CoreFunctions.h"

#include "../Function.h"

FunctionBase::FunctionBase(unsigned int Input, unsigned int Output) : InputDim(Input), OutputDim(Output)
{
    if (Input == 0 || Output == 0)
        throw std::logic_error("Cannot construct a function with an input or output of zero.");
}

[[nodiscard]] bool FunctionBase::ComparesTo(const FunctionBase* Obj) const
{
    if (!Obj)
        return false;

    bool Return = Obj->InputDim() == InputDim() && Obj->OutputDim() == OutputDim() && Obj->AllowedChildCount() == AllowedChildCount();
    if (Return && AllowedChildCount() != 0)
    {
        for (FunctionRelationSeg* CurrentH = First, *CurrentO = Obj->First; CurrentH != nullptr && CurrentO != nullptr; CurrentH = CurrentH->Next, CurrentO = CurrentO->Next)
        {
            Return &= CurrentH->Target && CurrentO->Target && CurrentH->Target->ComparesTo(CurrentO->Target);
            if (!Return)
                return false;
        }
    }

    return Return;
}
[[nodiscard]] bool FunctionBase::EquatesTo(const FunctionBase* Obj) const
{
    if (!Obj)
        return false;

    if (this->InputDim != Obj->InputDim || this->OutputDim != Obj->OutputDim || this->AllowedChildCount() != Obj->AllowedChildCount() || this->A != A)
        return false;

    if (AllowedChildCount() != 0)
    {
        const auto* tC = this->First, *oC = Obj->First;
        for (; tC && oC; tC = tC->Next, oC = oC->Next)
        {
            if (!tC->Target->ComparesTo(oC->Target));
        }
    }
    if (Return && AllowedChildCount() != 0)
    {
        for (FunctionRelationSeg* CurrentH = First, *CurrentO = Obj->First; CurrentH != nullptr && CurrentO != nullptr; CurrentH = CurrentH->Next, CurrentO = CurrentO->Next)
        {
            Return &= CurrentH->Target && CurrentO->Target && CurrentH->Target->EquatesTo(CurrentO->Target);
            if (!Return)
                return false;
        }
    }

    return Return;
}

FunctionBase& FunctionBase::operator-()
{
    A = -A; return *this;
}
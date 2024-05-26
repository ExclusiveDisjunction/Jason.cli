#include "FunctionBase.h"

#include "CompositeFunction.h"
#include "CoreFunctions.h"

#include "Function.h"

namespace Math::Function
{
	FunctionBase::FunctionBase(unsigned int Input, unsigned int Output) : _Input(Input), _Output(Output)
	{
		if (Input == 0 || Output == 0)
			throw std::exception("The input or output cannot be zero.");
	}

	unsigned int FunctionBase::InputDim() const
	{
		return _Input;
	}
	unsigned int FunctionBase::OutputDim() const
	{
		return _Output;
	}

	bool FunctionBase::ComparesTo(FunctionBase* const& Obj) const
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
	bool FunctionBase::EquatesTo(FunctionBase* const& Obj) const
	{
		if (!Obj)
			return false;

		bool Return = Obj->InputDim() == InputDim() && Obj->OutputDim() == OutputDim() && Obj->AllowedChildCount() == AllowedChildCount() && Obj->A == A;
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
}
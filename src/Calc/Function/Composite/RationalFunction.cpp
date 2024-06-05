#include "RationalFunction.h"

namespace Math::Function
{
	RationalFunction::RationalFunction(unsigned int InputDim) : CompositeFunction(InputDim, 1)
	{

	}
	RationalFunction::RationalFunction(FunctionBase* Obj) : CompositeFunction(!Obj ? 0 : Obj->InputDim(), 1)
	{
		MultiplyFunction(Obj);
	}
	RationalFunction::~RationalFunction()
	{

	}

	void RationalFunction::MultiplyFunction(FunctionBase* Obj)
	{
		if (!Obj || Obj->InputDim() != InputDim() || Obj->OutputDim() != 1)
			return;

		FunctionRelationSeg* Seg = new FunctionRelationSeg(Obj, nullptr, nullptr);
		AssignParent(Obj);
		PushChild(Seg);
	}
	void RationalFunction::DivideFunction(FunctionBase* Obj)
	{
		if (!Obj || Obj->InputDim() != InputDim() || Obj->OutputDim() != 1)
			return;

		FunctionRelationSeg* Seg = new FunctionRelationSeg(Obj, nullptr, nullptr);
		AssignParent(Obj);
		Seg->Flag |= RationalFuncFlag::Inverted;

		PushChild(Seg);
	}

	MathVector RationalFunction::Evaluate(const MathVector& X, bool& Exists) const
	{
		Exists = true;
		if (this->Size == 0)
		{
			Exists = false;
			return MathVector::ErrorVector();
		}

		double Return = 1;
		for (FunctionRelationSeg* Current = First; Current != nullptr; Current = Current->Next)
		{
			if (!Current->Target || Current->Target->InputDim() != InputDim())
			{
				Exists = false;
				return MathVector::ErrorVector();
			}

			MathVector Result = Current->Target->Evaluate(X, Exists);
			if (!Exists)
				return MathVector::ErrorVector();

			double Value = Result;
			if (Current->Flag & RationalFuncFlag::Inverted)
				Value = 1 / Value;

			Return *= Value;
		}

		return MathVector(1, Return);
	}

	bool RationalFunction::EquatesTo(FunctionBase* const& Obj) const
	{
		RationalFunction* Conv = dynamic_cast<RationalFunction*>(Obj);
		if (!Conv)
			return false;

		if (Conv->InputDim() != InputDim() || Conv->Size != Size)
			return false;

		bool Return = true;
		for (FunctionRelationSeg* CurrentH = First,* CurrentO = Conv->First; CurrentH != nullptr && CurrentO != nullptr; CurrentH = CurrentH->Next, CurrentO = CurrentO->Next)
			Return &= CurrentH->Target && CurrentH->Target->EquatesTo(CurrentO->Target) && CurrentH->Flag == CurrentO->Flag;
		
		return Return;
	}
	bool RationalFunction::ComparesTo(FunctionBase* const& Obj) const
	{
		RationalFunction* Conv = dynamic_cast<RationalFunction*>(Obj);
		if (!Conv)
			return false;

		if (Conv->InputDim() != InputDim() || Conv->Size != Size)
			return false;

		bool Return = true;
		for (FunctionRelationSeg* CurrentH = First, *CurrentO = Conv->First; CurrentH != nullptr && CurrentO != nullptr; CurrentH = CurrentH->Next, CurrentO = CurrentO->Next)
			Return &= CurrentH->Target && CurrentH->Target->ComparesTo(CurrentO->Target) && CurrentH->Flag == CurrentO->Flag;

		return Return;
	}
	FunctionBase* RationalFunction::Clone() const
	{
		if (Size == 0)
			return new RationalFunction(InputDim());

		RationalFunction* Return = new RationalFunction(InputDim());
		FunctionRelationSeg* Current, * Temp = nullptr;
		for (Current = this->First; Current != nullptr; Current = Current->Next)
		{
			FunctionRelationSeg* Other = new FunctionRelationSeg(Current->Target, nullptr, nullptr);
			Other->Flag = Current->Flag;

			if (Temp)
				Temp->Next = Other;
			Other->Previous = Temp;

			if (Return->First != nullptr)
				Return->First = Other;

			Temp = Other;
			Return->Size++;
		}

		Return->Last = Temp;
		return Return;
	}
}
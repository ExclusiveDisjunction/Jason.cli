#include "Polynomial.h"

namespace Math::Function
{
	Polynomial::Polynomial(unsigned int InputDim, unsigned int OutputDim) : CompositeFunction(InputDim, OutputDim)
	{

	}
	Polynomial::Polynomial(FunctionBase* Obj) : CompositeFunction(!Obj ? 0 : Obj->InputDim(), !Obj ? 0 : Obj->OutputDim())
	{
		if (!Obj)
			throw std::exception("The provided function cannot be nullptr.");

		AddFunction(Obj);
	}
	Polynomial::~Polynomial()
	{

	}

	void Polynomial::AddFunction(FunctionBase* Obj)
	{
		if (!Obj || Obj->InputDim() != InputDim())
			return;

		FunctionRelationSeg* Seg = new FunctionRelationSeg(Obj, nullptr, nullptr);
		AssignParent(Obj);
		PushChild(Seg);
	}
	void Polynomial::SubtractFunction(FunctionBase* Obj)
	{
		if (!Obj || Obj->InputDim() != InputDim() || Obj->OutputDim() != OutputDim())
			return;

		FunctionRelationSeg* Seg = new FunctionRelationSeg(Obj, nullptr, nullptr);
		AssignParent(Obj);
		Seg->Flag = PolynomialFlag::Negated;

		PushChild(Seg);
	}

	MathVector Polynomial::Evaluate(const MathVector& Obj, bool& Exists) const
	{
		Exists = true;
		if (this->Size == 0)
		{
			Exists = false;
			return MathVector::ErrorVector();
		}

		MathVector Output(OutputDim());
		for (FunctionRelationSeg* Current = First; Current != nullptr; Current = Current->Next)
		{
			if (!Current->Target || Current->Target->OutputDim() != OutputDim())
			{
				Exists = false;
				return MathVector::ErrorVector();
			}

			MathVector Result = Current->Target->Evaluate(Obj, Exists);
			if (!Exists)
				return MathVector::ErrorVector();

			Result *= static_cast<double>(Current->Flag & PolynomialFlag::Negated ? -1 : 1);
			Output += Result;
		}

		return Output;
	}

	bool Polynomial::EquatesTo(FunctionBase* const& Obj) const
	{
		Polynomial* Conv = dynamic_cast<Polynomial*>(Obj);
		if (!Conv)
			return false;

		if (Conv->InputDim() != InputDim() || Conv->Size != Size)
			return false;

		bool Return = true;
		for (FunctionRelationSeg* CurrentH = First, *CurrentO = Conv->First; CurrentH != nullptr && CurrentO != nullptr; CurrentH = CurrentH->Next, CurrentO = CurrentO->Next)
			Return &= CurrentH->Target && CurrentH->Target->EquatesTo(CurrentO->Target) && CurrentH->Flag == CurrentO->Flag;

		return Return;
	}
	bool Polynomial::ComparesTo(FunctionBase* const& Obj) const
	{
		Polynomial* Conv = dynamic_cast<Polynomial*>(Obj);
		if (!Conv)
			return false;

		if (Conv->InputDim() != InputDim() || Conv->Size != Size)
			return false;

		bool Return = true;
		for (FunctionRelationSeg* CurrentH = First, *CurrentO = Conv->First; CurrentH != nullptr && CurrentO != nullptr; CurrentH = CurrentH->Next, CurrentO = CurrentO->Next)
			Return &= CurrentH->Target && CurrentH->Target->ComparesTo(CurrentO->Target) && CurrentH->Flag == CurrentO->Flag;

		return Return;
	}

	FunctionBase* Polynomial::Clone() const
	{
		if (Size == 0)
			return new Polynomial(InputDim(), OutputDim());

		Polynomial* Return = new Polynomial(InputDim(), OutputDim());
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
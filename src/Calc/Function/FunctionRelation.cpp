#include "FunctionRelation.h"
#include "FunctionBase.h"

namespace Math::Function
{
	FunctionRelationSeg::FunctionRelationSeg(FunctionBase* Target, FunctionRelationSeg* Next, FunctionRelationSeg* Previous) : Next(Next), Previous(Previous), Target(Target)
	{

	}
	FunctionRelationSeg::~FunctionRelationSeg()
	{
		if (DeleteOnDeallocate)
		{
			if (Target)
				delete Target;

			Target = nullptr;
		}

		Next = Previous = nullptr;
	}

	FunctionRelation::FunctionRelation()
	{
		First = nullptr;
		Last = nullptr;
		ParentObject = nullptr;
		Size = 0;
	}
	FunctionRelation::~FunctionRelation()
	{
		if (Size != 0)
		{
			for (FunctionRelationSeg* Current = First; Current != nullptr;)
			{
				FunctionRelationSeg* Next = Current ? Current->Next : nullptr;
				delete Current;

				Current = Next;
			}

			Size = 0;
		}
	}

	void FunctionRelation::AssignParent(FunctionBase* Child)
	{
		if (Child == nullptr)
			return;

		if (Child->ParentObject != nullptr)
			Child->ParentObject->RemoveChild(Child, false);
		
		Child->ParentObject = this;
		PushChild(new FunctionRelationSeg(Child, nullptr, nullptr));
	}
	void FunctionRelation::PushChild(FunctionRelationSeg* Seg)
	{
		if (!Seg || !Seg->Target)
			return;

		if (!Last)
		{
			First = Last = Seg;
			Seg->Next = nullptr;
			Seg->Previous = nullptr;
			Size++;
			return;
		}
		else if (First == Last || Size == 1)
		{
			Last = Seg;
			Seg->Previous = First;
			First->Next = Seg;
			Size++;
			return;
		}

		Last->Next = Seg;
		Seg->Previous = Last;
		Last = Seg;
		ChildAdded(Seg);
		Size++;
	}
	void FunctionRelation::PopChild(FunctionRelationSeg* Seg, bool DeleteFunc)
	{
		if (!Seg || !Seg->Target || Seg->Target->ParentObject != this)
			return;

		if (Size == 1 && First == Seg && Last == Seg)
			First = Last = nullptr;
		else if (Seg == First)
			First = First->Next;
		else if (Seg == Last)
			Last = Last->Previous;
		else
		{
			FunctionRelationSeg* Next = Seg->Next, * Prev = Seg->Previous;
			if (Next)
				Next->Previous = Prev;
			if (Prev)
				Prev->Next = Next;

		}
		
		Size--;
		ChildRemoved(Seg->Target);
		if (DeleteFunc)
			delete Seg;
		else
		{
			Seg->Target = nullptr;
			delete Seg;
		}
	}

	FunctionRelationSeg* FunctionRelation::Retrive(unsigned int i) const
	{
		if (i >= Size)
			return nullptr;

		if (i == 0)
			return First;
		else if (i == Size - 1)
			return Last;

		unsigned int ci = 0;
		FunctionRelationSeg* Current = First;
		for (unsigned int ci = 0; Current != nullptr && ci < i; ci++)
			Current = Current->Next;

		return Current;
	}
	FunctionRelationSeg* FunctionRelation::Retrive(FunctionBase* Obj) const
	{
		return (!Obj || Obj->ParentObject != this) ? nullptr : Obj->HostSeg;
	}

	void FunctionRelation::RemoveParent()
	{
		if (!HostSeg)
			return;

		ParentObject->PopChild(HostSeg, false);
	}
	void FunctionRelation::AddChild(FunctionBase* Child)
	{
		if (!Child)
			return;

		PushChild(new FunctionRelationSeg(Child, nullptr, nullptr));
	}
	void FunctionRelation::RemoveChild(FunctionBase* Child, bool Delete)
	{
		if (!Child || Child->ParentObject != this)
			return;

		PopChild(Child->HostSeg, Delete);
	}
}
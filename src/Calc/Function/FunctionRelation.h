#pragma once

#include "..\StdCalc.h"

namespace Math::Function
{
	class MATH_LIB FunctionBase;

	class MATH_LIB FunctionRelationSeg
	{
	public:
		FunctionRelationSeg(FunctionBase* Target, FunctionRelationSeg* Next, FunctionRelationSeg* Previous);
		FunctionRelationSeg(const FunctionRelationSeg& Obj) = delete;
		FunctionRelationSeg(FunctionRelationSeg&& Obj) = delete;
		~FunctionRelationSeg();

		FunctionRelationSeg& operator=(const FunctionRelationSeg& Obj) = delete;
		FunctionRelationSeg& operator=(FunctionRelationSeg&& Obj) = delete;

		bool DeleteOnDeallocate = true;

		FunctionRelationSeg* Next = nullptr, * Previous = nullptr;
		FunctionBase* Target = nullptr;
		unsigned char Flag = 0;
	};

	class MATH_LIB FunctionRelation
	{
	protected:
		FunctionRelation();
		FunctionRelation(const FunctionRelation& Obj) = delete;
		FunctionRelation(FunctionRelation&& Obj) = delete;

		FunctionRelationSeg* First = nullptr, * Last = nullptr;
		unsigned int Size = 0;

		void AssignParent(FunctionBase* Child); //Makes the current inheriting class of this base class the parent of the function, and if the child has a previous owner, removes it from the other function. 
		void PushChild(FunctionRelationSeg* NewSeg);
		void PopChild(FunctionRelationSeg* OldSeg, bool DeleteFunc = true); //NOTE: If DeleteFunc is false, ONLY the FunctionRelationSeg holding the function is deleted, while the function is retained. 

		virtual void ChildRemoved(FunctionBase* Child) = 0;		
		virtual void ChildAdded(FunctionRelationSeg* Segment) {}

		FunctionRelationSeg* Retrive(unsigned int i) const;
		FunctionRelationSeg* Retrive(FunctionBase* Obj) const;
	public:
		virtual ~FunctionRelation();

		FunctionRelation& operator=(const FunctionRelation& Obj) = delete;
		FunctionRelation& operator=(FunctionRelation&& Obj) = delete;

		FunctionRelation* ParentObject = nullptr;
		FunctionRelationSeg* HostSeg = nullptr;

		bool CanHaveChildren() const { return AllowedChildCount() != 0; }
		virtual unsigned int AllowedChildCount() const = 0;
		virtual bool AllowsChildAppend() const = 0;
		void RemoveParent();

		virtual void AddChild(FunctionBase* Child);
		virtual void RemoveChild(FunctionBase* Child, bool Delete = true);
		unsigned int ChildCount() const { return Size; }
	};
}
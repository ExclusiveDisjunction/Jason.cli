#pragma once

#include "FunctionBase.h"

#include <iostream>

namespace Math::Function
{
	class MATH_LIB CompositeFunction : public FunctionBase
	{
	private:
		void ChildRemoved(FunctionBase* Child) override {}

	protected:
		void AddChild(FunctionBase* Child) override
		{
			FunctionRelationSeg* New = new FunctionRelationSeg(Child, nullptr, nullptr);
			PushChild(New);
		}

	public:
		CompositeFunction(unsigned int InputDim, unsigned int OutputDim);
		CompositeFunction(const CompositeFunction& Obj) = delete;
		CompositeFunction(const CompositeFunction&& Obj) = delete;

		CompositeFunction& operator=(const CompositeFunction& Obj) = delete;
		CompositeFunction& operator=(const CompositeFunction&& Obj) = delete;

		FunctionBase* operator[](unsigned int i) const;
		FunctionBase* Item(unsigned int i) const { return operator[](i); }

		unsigned int AllowedChildCount() const override { return UINT_MAX; }
		bool AllowsChildAppend() const override { return true; }
	};
}
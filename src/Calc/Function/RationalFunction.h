#pragma once

#include "FunctionBase.h"
#include "CompositeFunction.h"

#include <iostream>

namespace Math::Function
{
	class MATH_LIB RationalFunction : public CompositeFunction
	{
	public:
		enum RationalFuncFlag
		{
			Inverted = 1
		};

		RationalFunction(unsigned int InputDim);
		RationalFunction(FunctionBase* Obj);
		template<std::convertible_to<FunctionBase*>... Args>
		RationalFunction(unsigned int Dim, Args... Objs) : CompositeFunction(Dim, 1)
		{
			std::vector<FunctionBase*> Func = std::vector<FunctionBase*>({ (static_cast<FunctionBase*>(Objs))... });
			if (Func.size() == 0)
				return;

			for (FunctionBase* Obj : { Func... })
			{
				if (Obj->InputDim() != Dim || Obj->OutputDim() != 1)
					continue;

				MultiplyFunction(Obj);
			}
		}
		~RationalFunction();

		void MultiplyFunction(FunctionBase* Obj);
		template<std::convertible_to<FunctionBase*>... Args>
		void MultiplyFunctions(Args... Obj)
		{
			unsigned int Dim = InputDim();

			FunctionRelationSeg* Current = Last;
			for (FunctionBase* Item : { ((FunctionBase*)Obj)... })
			{
				if (!Item || Item->InputDim() != Dim)
					continue;

				FunctionRelationSeg* Seg = new FunctionRelationSeg(Obj, nullptr, nullptr);
				AssignParent(Obj);

				if (Current)
					Current->Next = Seg;
				Seg->Previous = Current;

				if (First == nullptr)
					First = Seg;
				
				Current = Seg;
				Size++;
			}

			Last = Current;
		}
		void DivideFunction(FunctionBase* Obj);

		MathVector Evaluate(const MathVector& X, bool& Exists) const override;
		
		bool EquatesTo(FunctionBase* const& Obj) const override;
		bool ComparesTo(FunctionBase* const& Obj) const override;
		FunctionBase* Clone() const override;
	};
}
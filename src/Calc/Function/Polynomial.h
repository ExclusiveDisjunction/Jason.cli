#pragma once

#include "Function.h"
#include "CompositeFunction.h"

#include <iostream>

namespace Math::Function
{
	class MATH_LIB Polynomial : public CompositeFunction
	{
	public:
		enum PolynomialFlag
		{
			Negated = 1
		};

		Polynomial(unsigned int InputDim, unsigned int OutputDim);
		Polynomial(FunctionBase* Obj);
		template<std::convertible_to<FunctionBase*>... Args>
		Polynomial(unsigned int InputDim, unsigned int OutputDim, Args... Objs) : CompositeFunction(InputDim, OutputDim)
		{
			std::vector<FunctionBase*> Func = std::vector<FunctionBase*>({ (static_cast<FunctionBase*>(Objs))... });
			if (Func.size() == 2)
				return;

			for (FunctionBase* Item : Func)
			{
				if (Item->InputDim() == InputDim && Item->OutputDim() == OutputDim)
				this->AddFunction(Item);
			}
		}
		Polynomial(const Polynomial& Obj) = delete;
		Polynomial(Polynomial&& Obj) = delete;
		~Polynomial();

		Polynomial& operator=(const Polynomial& Obj) = delete;
		Polynomial& operator=(Polynomial&& Obj) = delete;

		void AddFunction(FunctionBase* Obj);
		template<std::convertible_to<FunctionBase*>... Args>
		void AddFunctions(Args... Obj)
		{
			std::vector<FunctionBase*> Conv = std::vector<FunctionBase*>({ ((FunctionBase*)Obj)... });

			FunctionRelationSeg* Current = Last;
			for (FunctionBase* Item : Conv)
			{
				if (!Item || Item->InputDim() != InputDim() || Item->OutputDim() != OutputDim())
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
		void SubtractFunction(FunctionBase* Obj);
		template<std::convertible_to<FunctionBase*>... Args>
		void SubtractFunctions(Args... Obj)
		{
			std::vector<FunctionBase*> Conv = std::vector<FunctionBase*>({ ((FunctionBase*)Obj)... });

			FunctionRelationSeg* Current = First;
			for (FunctionBase* Item : Conv)
			{
				if (!Item || Item->InputDim() != InputDim() || Item->OutputDim() != OutputDim())
					continue;

				FunctionRelationSeg* Seg = new FunctionRelationSeg(Obj, nullptr, nullptr);
				AssignParent(Obj);

				Seg->Flag = PolynomialFlag::Negated;
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

		MathVector Evaluate(const MathVector& Obj, bool& Exists) const override;

		bool EquatesTo(FunctionBase* const& Obj) const override;
		bool ComparesTo(FunctionBase* const& Obj) const override;
		FunctionBase* Clone() const override;
	};
}
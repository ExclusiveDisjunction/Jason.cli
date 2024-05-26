#pragma once

#include "FunctionBase.h"
#include "SequenceBase.h"

namespace Math::Function
{
	class MATH_LIB VectorFunction : public FunctionBase
	{
	private:
		void Fill(unsigned int OutputDim);
		void Clear();

		FunctionBase** Func;
	protected:
		void ChildRemoved(FunctionBase* Item) override;

		void AddChild(FunctionBase* Child) override {}
		void RemoveChild(FunctionBase* Child, bool Delete = true) override {}
	public:
		VectorFunction(unsigned int InputDim, unsigned int OutputDim);
		VectorFunction(unsigned int InputDim, unsigned int OutputDim, FunctionBase** ToHost);
		VectorFunction(unsigned int InputDim, unsigned int OutputDim, Core::SequenceBase<FunctionBase*>* ToHost);
		template<std::convertible_to<FunctionBase*>... Args>
		VectorFunction(unsigned int InputDim, unsigned int OutputDim, Args... Value) : FunctionBase(InputDim, OutputDim)
		{
			Fill(OutputDim);

			std::vector<FunctionBase*> ToFill = std::vector<FunctionBase*>({ (static_cast<FunctionBase*>(Value))... });
			if (ToFill.size() != OutputDim)
				return;

			for (unsigned int i = 0; i < OutputDim; i++)
			{
				if (!ToFill[i] || ToFill[i]->InputDim() != _Input || ToFill[i]->OutputDim() != 1)
					continue;

				AssignParent(ToFill[i]);
				Func[i] = ToFill[i];
			}
		}
		VectorFunction(const VectorFunction& Obj) = delete;
		VectorFunction(VectorFunction&& Obj) = delete;
		virtual ~VectorFunction();

		VectorFunction& operator=(const VectorFunction& Obj) = delete;
		VectorFunction& operator=(VectorFunction&& Obj) = delete;

		void AssignFunction(unsigned int Index, FunctionBase* Func);
		FunctionBase* operator[](unsigned int Index) const;

		virtual MathVector Evaluate(const MathVector& In, bool& Exists) const override;

		unsigned int AllowedChildCount() const override { return _Output; }
		bool AllowsChildAppend() const override { return false; }

		bool ComparesTo(FunctionBase* const& Obj) const override;
		bool EquatesTo(FunctionBase* const& Obj) const override;
		FunctionBase* Clone() const override;

		bool operator==(const VectorFunction& Obj)
		{
			return Obj.EquatesTo(this);
		}
		bool operator!=(const VectorFunction& Obj)
		{
			return !Obj.EquatesTo(this);
		}
	};
}
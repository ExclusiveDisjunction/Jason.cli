#pragma once

#include "FunctionRelation.h"
#include "..\StdCalc.h"
#include "..\MathVector.h"
#include "..\Matrix.h"

namespace Math::Function
{
	class MATH_LIB FunctionBase : virtual public FunctionRelation
	{
	protected:
		const unsigned int _Input;
		const unsigned int _Output;

		FunctionBase(unsigned int InputDim, unsigned int OutputDim);
	public:
		FunctionBase(const FunctionBase& Obj) = delete;
		FunctionBase(FunctionBase&& Obj) = delete;
		virtual ~FunctionBase() {}

		FunctionBase& operator=(const FunctionBase& Obj) = delete;
		FunctionBase& operator=(FunctionBase&& Obj) = delete;

		double A = 1.0;

		unsigned int InputDim() const;
		unsigned int OutputDim() const;

		virtual MathVector Evaluate(const MathVector& X, bool& Exists) const = 0;

		virtual bool ComparesTo(FunctionBase* const& Obj) const;
		virtual bool EquatesTo(FunctionBase* const& Obj) const;
		virtual FunctionBase* Clone() const = 0;

		virtual FunctionBase& operator-();
	};
}
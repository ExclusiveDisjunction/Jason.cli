#pragma once

#include "..\StdCalc.h"
#include "Function.h"
#include "Polynomial.h"

namespace Math::Function
{
	class MATH_LIB BezierMonomial : public FunctionBase
	{
	public:
		BezierMonomial(unsigned int Dim, int i, int n, MathVector Target);

		MathVector Point;
		int i = 0;
		int n = 0;

		MathVector Evaluate(const MathVector& T, bool& Exists) const override;

		bool AllowsChildAppend() const override { return false; }
		unsigned int AllowedChildCount() const override { return 0; }
		void ChildRemoved(FunctionBase* Child) override { }
		FunctionBase* Clone() const override;
	};

	class Bezier : public FunctionBase
	{
	private:
		unsigned int Rank = 0;
		Polynomial* Segments = nullptr;
	public:
		Bezier(unsigned int Dim, unsigned int Rank);
		Bezier(unsigned int Dim, unsigned int Rank, MathVector Obj[]);
		virtual ~Bezier();

		MathVector Evaluate(const MathVector& T, bool& Exists) const override;
	};
}
#include "Bezier.h"

namespace Math::Function
{
	BezierMonomial::BezierMonomial(unsigned int Dim, int i, int n, MathVector Target) : FunctionBase(1, Dim), Point(Target)
	{
		if (Target.Dim() != Dim)
			throw std::exception("The input dimensions does not match the vector's input dimensions.");

		this->i = i;
		this->n = n;
		float fac = 1.0f;
		for (int j = 1; j <= i; j++)
			fac *= j;
		this->A = (pow(n, i)) / (fac);
	}

	MathVector BezierMonomial::Evaluate(const MathVector& T, bool& Exists) const
	{
		if (T.Dim() != 1)
		{
			Exists = false;
			return MathVector::ErrorVector();
		}

		return this->A * pow(1 - T[0], n - 1) * pow(T[0], i) * Point;
	}
	FunctionBase* BezierMonomial::Clone() const
	{
		BezierMonomial* Return = new BezierMonomial(this->OutputDim(), i, n, this->Point);
		Return->A = this->A;
		return Return;
	}

	Bezier::Bezier(unsigned int Dim, unsigned int Rank) : FunctionBase(1, Dim)
	{
		//No other items needed.
	}
	Bezier::Bezier(unsigned int Dim, unsigned int Rank, MathVector Obj[]) : FunctionBase(1, Dim)
	{
		Segments = new Polynomial(1, Dim);
		for (int i = 0; i < Rank; i++)
			Segments->AddFunction(new BezierMonomial(Dim, i, Rank, Obj[i]));
	}
	Bezier::~Bezier()
	{
		delete Segments;
	}

	MathVector Bezier::Evaluate(const MathVector& T, bool& Exists) const
	{
		if (T.Dim() != 1 || T < 0.0f || T > 1.0f)
		{
			Exists = false;
			return MathVector::ErrorVector();
		}

		Exists = true;
		return Segments->Evaluate(T, Exists);
	}
}
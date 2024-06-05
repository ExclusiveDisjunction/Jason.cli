#include "..\CoreFunctions.h"

namespace Math::Function
{
	Monomial::Monomial(unsigned int InputDim, unsigned int Var, double A, double N) : FunctionBase(InputDim, 1)
	{
		this->N = N;
		this->VarLetter = Var;
		this->A = A;
	}

	void Monomial::ChildRemoved(FunctionBase* Obj)
	{
		//Monomials cannot have children. 
	}

	MathVector Monomial::Evaluate(const MathVector& X, bool& Exists) const
	{
		if (X.Dim() != InputDim())
		{
			Exists = false;
			return MathVector::ErrorVector();
		}

		return A * pow(X[VarLetter], N);
	}

	FunctionBase* Monomial::Clone() const
	{
		return new Monomial(InputDim(), VarLetter, A, N);
	}
}
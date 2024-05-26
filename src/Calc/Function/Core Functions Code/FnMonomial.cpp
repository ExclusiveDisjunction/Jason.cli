#include "..\CoreFunctions.h"

namespace Math::Function
{
	FnMonomial::FnMonomial(unsigned int InputDim, unsigned int Var) : FnMonomial(new Monomial(InputDim, Var), 1, 1)
	{

	}
	FnMonomial::FnMonomial(FunctionBase* InnerFunction, double A, double N) : FunctionBase(!InnerFunction ? 0 : InnerFunction->InputDim(), 1)
	{
		if (!InnerFunction || InnerFunction->OutputDim() != 1)
			throw std::exception("The inner function must be valid, and the output dimension must be one.");

		this->A = A;
		this->N = N;
		_B = InnerFunction;
		AssignParent(_B);
	}

	FunctionBase* FnMonomial::Base() const
	{
		return _B;
	}
	void FnMonomial::Base(FunctionBase* const& NewB)
	{
		if (!NewB || NewB->InputDim() != InputDim() || NewB->OutputDim() != 1)
			throw std::exception("The inner function must exist, and the input dimension must be the input dimension of this function, as well as the output dimension of the function being 1.");
		if (_B)
		{
			FunctionBase* Temp = _B;
			_B->RemoveParent();
			delete Temp;
		}
		
		_B = NewB;
		AssignParent(_B);
	}

	void FnMonomial::ChildRemoved(FunctionBase* Obj)
	{
		if (_B == Obj)
			_B = nullptr;
	}
	
	MathVector FnMonomial::Evaluate(const MathVector& X, bool& Exists) const
	{
		if (X.Dim() != InputDim() || !_B)
		{
			Exists = false;
			return MathVector::ErrorVector();
		}

		Exists = true;
		MathVector InnerEval = _B->Evaluate(X, Exists);
		if (!Exists)
			return MathVector::ErrorVector();

		return A * pow(InnerEval[0], N);
	}

	FunctionBase* FnMonomial::Clone() const
	{
		if (!_B)
			return nullptr;

		return new FnMonomial(_B->Clone(), A, N);
	}
}
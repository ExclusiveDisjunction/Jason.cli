#include "..\CoreFunctions.h"

namespace Math::Function
{
	AbsoluteValue::AbsoluteValue(unsigned int InputDim, unsigned int Var, double A) : AbsoluteValue(new Monomial(InputDim, Var), A)
	{

	}
	AbsoluteValue::AbsoluteValue(FunctionBase* N, double A) : FunctionBase(!N ? 0 : N->InputDim(), 1)
	{
		if (!N)
			throw std::exception("The input function cannot be nullptr.");

		this->A = A;
		AssignParent(N);
		_N = N;
	}

	void AbsoluteValue::ChildRemoved(FunctionBase* Child)
	{
		if (Child == _N)
			_N = nullptr;
	}

	FunctionBase* AbsoluteValue::Base() const
	{
		return _N;
	}
	void AbsoluteValue::Base(FunctionBase* const& NewB)
	{
		if (!NewB || NewB->InputDim() != InputDim())
			throw std::exception("The new function must exist and have the same input dimension.");

		if (_N)
		{
			FunctionBase* Temp = _N;
			_N->RemoveParent(); //Remove parent calls FunctionRelation::ChildRemoved, which will set _N to nullptr.
			delete Temp;
		}
		
		_N = NewB;
		AssignParent(_N);
	}

	MathVector AbsoluteValue::Evaluate(const MathVector& X, bool& Exists) const
	{
		if (!_N || X.Dim() != InputDim())
		{
			Exists = false;
			return MathVector::ErrorVector();
		}

		Exists = true;
		MathVector Base = _N->Evaluate(X, Exists);
		if (!Exists)
			return MathVector::ErrorVector();

		return A * Base.Magnitude();
	}

	FunctionBase* AbsoluteValue::Clone() const
	{
		if (!_N)
			return nullptr;

		return new AbsoluteValue(_N->Clone(), A);
	}
}
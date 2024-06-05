#include "..\CoreFunctions.h"

namespace Math::Function
{
	Logarithm::Logarithm(double A, double B, FunctionBase* N) : FunctionBase(!N ? 0 : N->InputDim(), 1)
	{
		this->A = A;
		this->Base = B;

		if (!N || N->OutputDim() != 1)
			throw std::exception("The output dimension must be 1, and the function must exist");

		this->_N = N;
		AssignParent(_N);
	}
	Logarithm::Logarithm(double A, double B, unsigned int InputDim, unsigned int Var) : Logarithm(A, B, new Monomial(InputDim, Var))
	{

	}

	void Logarithm::ChildRemoved(FunctionBase* Child)
	{
		if (Child == _N)
			_N = nullptr;
	}

	FunctionBase* Logarithm::Function() const
	{
		return _N;
	}
	void Logarithm::Function(FunctionBase* const& Obj)
	{
		if (!Obj || Obj->InputDim() != InputDim() || Obj->OutputDim() != 1)
			throw std::exception("The input dimensions must match, the output dimension must be 1, and the function must exist.");

		if (_N)
		{
			FunctionBase* Temp = _N;
			_N->RemoveParent();
			delete Temp;
		}

		_N = Obj;
		AssignParent(_N);
	}

	MathVector Logarithm::Evaluate(const MathVector& X, bool& Exists) const
	{
		if (X.Dim() != InputDim() || !_N)
		{
			Exists = false;
			return MathVector::ErrorVector();
		}

		Exists = true;
		MathVector Result = _N->Evaluate(X, Exists);
		Exists &= !Exists ? false : Result[0] > 0.0; //Ensures that the log function does not recive a zero or negative value. 
		if (!Exists)
			return MathVector::ErrorVector();

		return A * log(Result[0]) / log(Base);
	}

	FunctionBase* Logarithm::Clone() const
	{
		if (!_N)
			return nullptr;

		return new Logarithm(A, Base, _N->Clone());
	}
}
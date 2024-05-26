#include "..\CoreFunctions.h"

namespace Math::Function
{
	PFnMonomial::PFnMonomial(unsigned int InputDim, FunctionBase* B, FunctionBase* N, double A) : FunctionBase(InputDim, 1)
	{
		if (!B || !N || B->InputDim() != InputDim || N->InputDim() != InputDim || B->OutputDim() != 1 || N->OutputDim() != 1)
			throw std::exception("The functions provided must have the following criteria: 1) Exist, 2) Their input dimension must match the provided input dimension, 3) Their output dimension must be one. One or more of these critera are not met.");

		_B = B;
		_N = N;
		AssignParent(B);
		AssignParent(N);
		this->A = A;
	}

	void PFnMonomial::ChildRemoved(FunctionBase* Obj)
	{
		if (Obj == _B)
			_B = nullptr;
		else if (Obj == _N)
			_N = nullptr;
	}

	FunctionBase* PFnMonomial::Base() const
	{
		return _B;
	}
	void PFnMonomial::Base(FunctionBase* const& Obj)
	{
		if (!Obj || Obj->InputDim() != InputDim() || Obj->OutputDim() != 1)
			throw std::exception("The input dimensions do not match, or the new functiond does not exist.");

		if (_B)
		{
			FunctionBase* Temp = _B;
			_B->RemoveParent();
			delete Temp;
		}

		_B = Obj;
		AssignParent(_B);
	}
	FunctionBase* PFnMonomial::Power() const
	{
		return _N;
	}
	void PFnMonomial::Power(FunctionBase* const& Obj)
	{
		if (!Obj || Obj->InputDim() != InputDim() || Obj->OutputDim() != 1)
			throw std::exception("The input dimensions do not match, or the new functiond does not exist.");

		if (_N)
		{
			FunctionBase* Temp = _N;
			_N->RemoveParent();
			delete Temp;
		}

		_N = Obj;
		AssignParent(_N);
	}

	MathVector PFnMonomial::Evaluate(const MathVector& X, bool& Exists) const
	{
		if (X.Dim() != InputDim() || !_B || !_N)
		{
			Exists = false;
			return MathVector::ErrorVector();
		}

		Exists = true;
		MathVector BaseV = _B->Evaluate(X, Exists);
		if (!Exists)
			return MathVector::ErrorVector();

		MathVector PowerV = _N->Evaluate(X, Exists);
		if (!Exists)
			return MathVector::ErrorVector();

		return A * pow(BaseV[0], PowerV[0]);
	}

	FunctionBase* PFnMonomial::Clone() const
	{
		if (!_B || !_N)
			return nullptr;

		return new PFnMonomial(InputDim(), _B->Clone(), _N->Clone(), A);
	}
}
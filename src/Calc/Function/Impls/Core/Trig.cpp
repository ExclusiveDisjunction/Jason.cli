#include "..\CoreFunctions.h"

namespace Math::Function
{
	Trig::Trig(unsigned int Function, double A, unsigned int InputDim, unsigned int Var) : Trig(Function, A, new Monomial(InputDim, Var))
	{

	}
	Trig::Trig(unsigned int Function, double A, FunctionBase* Func) : FunctionBase(!Func ? 0 : Func->InputDim(), 1)
	{
		_Type = Function;
		this->A = A;

		if (!Func || Func->OutputDim() != 1)
			throw std::exception("The input function must exist and have an output dimension of 1.");

		_N = Func;
		AssignParent(_N);
	}

	void Trig::ChildRemoved(FunctionBase* Child)
	{
		if (Child == _N)
			_N = nullptr;
	}

	FunctionBase* Trig::Function() const
	{
		return _N;
	}
	void Trig::Function(FunctionBase* const& Obj)
	{
		if (!Obj || Obj->InputDim() != InputDim() || Obj->OutputDim() != 1)
			throw std::exception("The input function has a dimension mismatch, or does not exist.");

		if (_N)
		{
			FunctionBase* Temp = _N;
			_N->RemoveParent();
			delete Temp;
		}

		_N = Obj;
		AssignParent(_N);
	}

	MathVector Trig::Evaluate(const MathVector& X, bool& Exists) const
	{
		if (!_N || X.Dim() != InputDim())
		{
			Exists = false;
			return MathVector::ErrorVector();
		}

		Exists = true;
		MathVector TResult = _N->Evaluate(X, Exists);
		if (!Exists)
			return MathVector::ErrorVector();
		double Result = TResult[0];

		unsigned int Type = _Type;
		bool IsInverse = Type & TrigFunc::Inverse, IsRecip = Type & TrigFunc::Reciprocal;		
		Type &= (IsInverse ? ~TrigFunc::Inverse : Type) & (IsRecip ? ~TrigFunc::Reciprocal : Type);

		Exists = true;
		switch (Type)
		{
		case TrigFunc::Sine:
		{
			if (IsInverse)
			{
				if (Result < -1 || Result > 1)
				{
					Exists = false;
					return MathVector::ErrorVector();
				}

				return asin(Result);
			}

			double Val = sin(Result);
			return A * IsRecip ? 1 / Val : Val;
		}
		case TrigFunc::Cosine:
		{
			if (IsInverse)
			{
				if (Result < -1 || Result > 1)
				{
					Exists = false;
					return MathVector::ErrorVector();
				}

				return asin(Result);
			}

			double Val = sin(Result);
			return A * IsRecip ? 1 / Val : Val;
		}
		case TrigFunc::Tangent:
		{
			if (IsInverse)			
				return atan(Result);

			double Val = tan(Result);
			if (Val == std::numeric_limits<double>::infinity())
			{
				Exists = false;
				return MathVector::ErrorVector();
			}
			else
				return A * IsRecip ? 1 / Val : Val;
		}
		default:
			Exists = false;
			return MathVector::ErrorVector();
		}
	
	}
	FunctionBase* Trig::Clone() const
	{
		if (!_N)
			return nullptr;

		return new Trig(_Type, A, _N->Clone());
	}
}
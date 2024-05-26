#include "..\CoreFunctions.h"

namespace Math::Function
{
	Exponent::Exponent(unsigned int InputDim, unsigned int Var, double A, double B) : Exponent(new Monomial(InputDim, Var), A, B)
	{

	}
	Exponent::Exponent(FunctionBase* N, double A, double B) : FunctionBase(!N ? 0 : N->InputDim(), 1)
	{
		if (!N)
			throw std::exception("The input function does not exist.");

		this->N = N;
		AssignParent(N);

		this->A = A;
		this->Base = B;
	}

	void Exponent::ChildRemoved(FunctionBase* Obj)
	{
		if (Obj == N)
			N = nullptr;
	}

	FunctionBase* Exponent::Power() const
	{
		return N;
	}
	void Exponent::Power(FunctionBase* const& NewFunction)
	{
		if (!NewFunction || NewFunction->InputDim() != InputDim())
			throw std::exception("The input function does not exist, or has a different input dimension.");

		if (N)
		{
			FunctionBase* Temp = N;
			N->RemoveParent();
			delete Temp;
		}

		N = NewFunction;
		AssignParent(N);
	}

	MathVector Exponent::Evaluate(const MathVector& X, bool& Exists) const
	{
		if (!N || X.Dim() != InputDim())
		{
			Exists = false;
			return MathVector::ErrorVector();
		}

		Exists = true;
		MathVector Result = N->Evaluate(X, Exists);
		if (!Exists)
			return MathVector::ErrorVector();

		return A * pow(Base, Result[0]);
	}

	FunctionBase* Exponent::Clone() const
	{
		if (!N)
			return nullptr;

		return new Exponent(N->Clone(), A, Base);
	}
}
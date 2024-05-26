#include "GeneralFunctions.h"

#include "CoreFunctions.h"
#include "Polynomial.h"
#include "..\Constants.h"

namespace Math::Function
{
	FunctionBase* General::Linear(unsigned int Dim, unsigned int Var, double M, double K)
	{
		if (K == 0)
			return new Monomial(Dim, Var, M, 1);
		else
			return new Polynomial(Dim, 1, new Monomial(Dim, Var, M, 1), new Constant(Dim, K));
	}
	FunctionBase* General::Quadratic(unsigned int Dim, unsigned int Var, double A, double B, double C)
	{
		if (B == 0 && C == 0)
			return new Monomial(Dim, Var, A, 2);

		Polynomial* Return = new Polynomial(Dim, 1);
		Return->AddFunction(new Monomial(Dim, Var, A, 2));
		if (B != 0)
			Return->AddFunction(new Monomial(Dim, Var, B, 1));
		if (C != 0)
			Return->AddFunction(new Constant(Dim, C));

		return Return;
	}
	FunctionBase* General::Cubic(unsigned int Dim, unsigned int Var, double A, double B, double C, double D)
	{
		if (B == 0 && C == 0 && D == 0)
			return new Monomial(Dim, Var, A, 3);

		Polynomial* Return = new Polynomial(Dim, 1);
		Return->AddFunction(new Monomial(Dim, Var, A, 3));
		if (B != 0)
			Return->AddFunction(new Monomial(Dim, Var, B, 2));
		if (C != 0)
			Return->AddFunction(new Monomial(Dim, Var, C, 1));
		if (D != 0)
			Return->AddFunction(new Constant(Dim, D));

		return Return;
	}
	FunctionBase* General::SquareRoot(unsigned int Dim, unsigned int Var, double A, double _N)
	{
		FunctionBase* InnerFunc = new Monomial(Dim, Var, 1, _N);
		return new FnMonomial(InnerFunc, A, 0.5);
	}
	FunctionBase* General::Sine(unsigned int Dim, unsigned int Var, double A, double w, double phi, double C)
	{
		FunctionBase* InnerFunction = phi != 0.0 ? static_cast<FunctionBase*>(new Polynomial(Dim, 1, new Monomial(Dim, Var, w, 1), new Constant(Dim, phi))) : static_cast<FunctionBase*>(new Monomial(Dim, Var, w, 1));

		return new Trig(TrigFunc::Cosine, A, InnerFunction);
	}
	FunctionBase* General::Cosine(unsigned int Dim, unsigned int Var, double A, double w, double phi, double C)
	{
		FunctionBase* InnerFunction = phi != 0.0 ? static_cast<FunctionBase*>(new Polynomial(Dim, 1, new Monomial(Dim, Var, w, 1), new Constant(Dim, phi))) : static_cast<FunctionBase*>(new Monomial(Dim, Var, w, 1));

		return new Trig(TrigFunc::Cosine, A, InnerFunction);
	}
}
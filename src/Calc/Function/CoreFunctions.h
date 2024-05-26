#pragma once

#include "FunctionBase.h"

namespace Math::Function
{
	class MATH_LIB Constant : public FunctionBase
	{
	private:
		MathVector _Data;

		void ChildRemoved(FunctionBase* Child) override;
	public:
		Constant(unsigned int InputDim, double A = 1.0);
		Constant(unsigned int InputDim, MathVector Data);
		template<std::convertible_to<double>... Args>
		Constant(unsigned int InputDim, unsigned int OutputDim, Args... Val) : FunctionBase(InputDim, OutputDim)
		{
			std::vector<double> Vals = std::vector<double>({ (static_cast<double>(Val))... });
			if (Vals.size() != OutputDim)
				return;

			for (unsigned int i = 0; i < OutputDim; i++)
				_Data[i] = Vals[i];
		}
		Constant(const Constant& Obj) = delete;
		Constant(Constant&& Obj) = delete;
		
		Constant& operator=(const Constant& Obj) = delete;
		Constant& operator=(Constant&& Obj) = delete;

		MathVector Evaluate(const MathVector& X, bool& Exists) const override;

		bool ComparesTo(FunctionBase* const& Obj) const override;
		bool EquatesTo(FunctionBase* const& Obj) const override;
		FunctionBase* Clone() const override;

		unsigned int AllowedChildCount() const override { return 0; }
		bool AllowsChildAppend() const override { return false; }
	};

	/*
	* NOTE:
	*
	* There are 3 variants of monomials. THIS IS INTENTIONAL.
	*
	* The different types are to save on memory and complexity depending on the situation. It is situational as to when to use each different kind of monomial.
	*/

	/// <summary>
	/// Represents a single monomial term with an A value, and N power, as well as a variable.
	/// </summary>
	class MATH_LIB Monomial : public FunctionBase
	{
	private:
		void ChildRemoved(FunctionBase* Obj) override;

	public:
		Monomial(unsigned int InputDim, unsigned int Var, double A = 1.0, double N = 1.0);
		Monomial(const Monomial& Obj) = delete;
		Monomial(Monomial&& Obj) = delete;

		Monomial& operator=(const Monomial& Obj) = delete;
		Monomial& operator=(Monomial&& Obj) = delete;

		double N = 1.0;
		unsigned int VarLetter = 0;

		MathVector Evaluate(const MathVector& X, bool& Exists) const override;

		FunctionBase* Clone() const override;

		unsigned int AllowedChildCount() const override { return 0; }
		bool AllowsChildAppend() const override { return false; }
	};

	/// <summary>
	/// Represents a single monomial term with an A value, an N power, and a B base function. (Function Monomial)
	/// </summary>
	class MATH_LIB FnMonomial : public FunctionBase
	{
	private:
		void ChildRemoved(FunctionBase* Obj) override;

		FunctionBase* _B = nullptr;
	public:
		FnMonomial(unsigned int InputDim, unsigned int Var);
		FnMonomial(FunctionBase* InnerFunction, double A, double N);
		FnMonomial(const FnMonomial& Obj) = delete;
		FnMonomial(FnMonomial&& Obj) = delete;

		FnMonomial& operator=(const FnMonomial& Obj) = delete;
		FnMonomial& operator=(FnMonomial&& Obj) = delete;

		FunctionBase* Base() const;
		void Base(FunctionBase* const& NewB);
		double N = 0.0f;

		MathVector Evaluate(const MathVector& X, bool& Exists) const override;

		FunctionBase* Clone() const override;

		unsigned int AllowedChildCount() const override { return 1; }
		bool AllowsChildAppend() const override { return false; }
	};

	/// <summary>
	/// Represents a single monomial term with an A value, an N function power, and B base function. (Power Function Monomial)
	/// </summary>
	class MATH_LIB PFnMonomial : public FunctionBase
	{
	private:
		void ChildRemoved(FunctionBase* Obj) override;

		FunctionBase* _B = nullptr, * _N = nullptr;
	public:
		PFnMonomial(unsigned int InputDim, FunctionBase* B, FunctionBase* N, double A);
		PFnMonomial(const PFnMonomial& Obj) = delete;
		PFnMonomial(PFnMonomial&& Obj) = delete;

		PFnMonomial& operator=(const PFnMonomial& Obj) = delete;
		PFnMonomial& operator=(PFnMonomial&& Obj) = delete;

		FunctionBase* Base() const;
		void Base(FunctionBase* const& NewB);
		FunctionBase* Power() const;
		void Power(FunctionBase* const& NewN);

		MathVector Evaluate(const MathVector& X, bool& Exists) const override;

		FunctionBase* Clone() const override;

		unsigned int AllowedChildCount() const override { return 2; }
		bool AllowsChildAppend() const override { return false; }
	};

	/// <summary>
	/// Represents an absolute value function (V shape function).
	/// </summary>
	class MATH_LIB AbsoluteValue : public FunctionBase
	{
	private:
		void ChildRemoved(FunctionBase* Child) override;

		FunctionBase* _N = nullptr;
	public:
		AbsoluteValue(unsigned int InputDim, unsigned int Var, double A);
		AbsoluteValue(FunctionBase* N, double A);
		AbsoluteValue(const AbsoluteValue& Obj) = delete;
		AbsoluteValue(AbsoluteValue& Obj) = delete;

		AbsoluteValue& operator=(const AbsoluteValue& Obj) = delete;
		AbsoluteValue& operator=(AbsoluteValue&& Obj) = delete;

		FunctionBase* Base() const;
		void Base(FunctionBase* const& NewN);

		MathVector Evaluate(const MathVector& X, bool& Exists) const override;

		FunctionBase* Clone() const override;

		unsigned int AllowedChildCount() const override { return 1; }
		bool AllowsChildAppend() const override { return false; }
	};

	/// <summary>
	/// Represents an exponental function of function base N and power B.
	/// </summary>
	class MATH_LIB Exponent : public FunctionBase
	{
	private:
		void ChildRemoved(FunctionBase* Obj) override;

		FunctionBase* N = nullptr;
	public:
		Exponent(unsigned int InputDim, unsigned int Var, double A, double B);
		Exponent(FunctionBase* N, double A, double B);
		Exponent(const Exponent& Obj) = delete;
		Exponent(Exponent&& Obj) = delete;

		Exponent& operator=(const Exponent& Obj) = delete;
		Exponent& operator=(Exponent&& Obj) = delete;

		double Base;
		FunctionBase* Power() const;
		void Power(FunctionBase* const& NewFunction);

		MathVector Evaluate(const MathVector& X, bool& Exists) const override;

		FunctionBase* Clone() const override;

		unsigned int AllowedChildCount() const override { return 1; }
		bool AllowsChildAppend() const override { return false; }
	};

	/// <summary>
	/// Represents a logarithim of base B, evaluating function N.
	/// </summary>
	class MATH_LIB Logarithm : public FunctionBase
	{
	private:
		void ChildRemoved(FunctionBase* Child) override;

		FunctionBase* _N = nullptr;
	public:
		Logarithm(double A, double B, FunctionBase* N);
		Logarithm(double A, double B, unsigned int InputDim, unsigned int Var);
		Logarithm(const Logarithm& Obj) = delete;
		Logarithm(Logarithm&& Obj) = delete;

		Logarithm& operator=(const Logarithm& Obj) = delete;
		Logarithm& operator=(Logarithm&& Obj) = delete;

		double Base;
		FunctionBase* Function() const;
		void Function(FunctionBase* const& Obj);

		MathVector Evaluate(const MathVector& X, bool& Exists) const override;

		FunctionBase* Clone() const override;

		unsigned int AllowedChildCount() const override { return 1; }
		bool AllowsChildAppend() const override { return false; }
	};

	enum TrigFunc
	{
		Sine = 0,
		Cosine = 1,
		Tangent = 2,
		Inverse = 4,//Use for arcsin/arccsc, arccos/arcsec, arctan/arccot
		Reciprocal = 8 //Use for csc, sec, cot
	};

	/// <summary>
	/// Represents the 3 common trig functions: sine, cosine, and tangent.
	/// </summary>
	class MATH_LIB Trig : public FunctionBase
	{
	private:
		void ChildRemoved(FunctionBase* Obj) override;

		unsigned int _Type;
		FunctionBase* _N = nullptr;
	public:
		Trig(unsigned int Function, double A, unsigned int InputDim, unsigned int Var);
		Trig(unsigned int Function, double A, FunctionBase* InputDim);
		Trig(const Trig& Obj) = delete;
		Trig(Trig&& Obj) = delete;
		
		Trig& operator=(const Trig& Obj) = delete;
		Trig& operator=(Trig&& Obj) = delete;

		unsigned int Type() const { return _Type; }
		FunctionBase* Function() const;
		void Function(FunctionBase* const& NewObj);

		MathVector Evaluate(const MathVector& X, bool& Exists) const override;

		FunctionBase* Clone() const override;

		unsigned int AllowedChildCount() const override { return 1; }
		bool AllowsChildAppend() const override { return false; }
	};
}
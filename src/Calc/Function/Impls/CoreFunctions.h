
#ifndef JASON_COREFUNCTIONS_H
#define JASON_COREFUNCTIONS_H

#include "../FunctionBase.h"

class MATH_LIB Constant : public FunctionBase
{
private:
    void ChildRemoved(FunctionBase* Child) noexcept override;

public:
    explicit Constant(unsigned int InputDim, double A = 1.0);
    Constant(const Constant& Obj) = delete;
    Constant(Constant&& Obj) = delete;

    Constant& operator=(const Constant& Obj) = delete;
    Constant& operator=(Constant&& Obj) = delete;

    MathVector Evaluate(const MathVector& X, bool& Exists) const noexcept override;

    bool ComparesTo(const FunctionBase* Obj) const noexcept override;
    bool EquatesTo(const FunctionBase* Obj) const noexcept override;
    [[nodiscard]] [[maybe_unused]] FunctionBase* Clone() const noexcept override;
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
    void ChildRemoved(FunctionBase* Obj) noexcept override { }

public:
    Monomial(unsigned int InputDim, unsigned int Var, double A = 1.0, double N = 1.0);
    Monomial(const Monomial& Obj) = delete;
    Monomial(Monomial&& Obj) = delete;

    Monomial& operator=(const Monomial& Obj) = delete;
    Monomial& operator=(Monomial&& Obj) = delete;

    double N = 1.0;
    unsigned int VarLetter = 0;

    MathVector Evaluate(const MathVector& X, bool& Exists) const noexcept override;

    [[nodiscard]] bool ComparesTo(const FunctionBase* Obj) const noexcept override;
    [[nodiscard]] bool EquatesTo(const FunctionBase* Obj) const noexcept override;
    [[nodiscard]] FunctionBase* Clone() const noexcept override;
};

/// <summary>
/// Represents a single monomial term with an A value, an N power, and a B base function. (Function Monomial)
/// </summary>
class MATH_LIB FnMonomial : public FunctionBase
{
private:
    void ChildRemoved(FunctionBase* Obj) noexcept override;

    FunctionBase* B = nullptr;
public:
    FnMonomial(FunctionBase* Base, double Power, double A);
    FnMonomial(const FnMonomial& Obj) = delete;
    FnMonomial(FnMonomial&& Obj) = delete;

    FnMonomial& operator=(const FnMonomial& Obj) = delete;
    FnMonomial& operator=(FnMonomial&& Obj) = delete;

    [[nodiscard]] const FunctionBase& Base() const;
    [[nodiscard]] FunctionBase& Base();
    void Base(FunctionBase* NewB);
    double N = 0.0;

    MathVector Evaluate(const MathVector& X, bool& Exists) const noexcept override;

    [[nodiscard]] bool ComparesTo(const FunctionBase* Obj) const noexcept override;
    [[nodiscard]] bool EquatesTo(const FunctionBase* Obj) const noexcept override;
    [[nodiscard]] FunctionBase* Clone() const noexcept override;
};

/// <summary>
/// Represents a single monomial term with an A value, an N function power, and B base function. (Power Function Monomial)
/// </summary>
class MATH_LIB PFnMonomial : public FunctionBase
{
private:
    void ChildRemoved(FunctionBase* Obj) noexcept override;

    FunctionBase* B = nullptr, * N = nullptr;
public:
    PFnMonomial(unsigned int InputDim, FunctionBase* B, FunctionBase* N, double A);
    PFnMonomial(const PFnMonomial& Obj) = delete;
    PFnMonomial(PFnMonomial&& Obj) = delete;

    PFnMonomial& operator=(const PFnMonomial& Obj) = delete;
    PFnMonomial& operator=(PFnMonomial&& Obj) = delete;

    [[nodiscard]] [[maybe_unused]] const FunctionBase& Base() const;
    [[nodiscard]] [[maybe_unused]] FunctionBase& Base();
    [[nodiscard]] [[maybe_unused]] const FunctionBase& Power() const;
    [[nodiscard]] [[maybe_unused]] FunctionBase& Power();

    void Base(FunctionBase* New);
    void Power(FunctionBase* New);

    MathVector Evaluate(const MathVector& X, bool& Exists) const noexcept override;

    [[nodiscard]] bool ComparesTo(const FunctionBase* Obj) const noexcept override;
    [[nodiscard]] bool EquatesTo(const FunctionBase* Obj) const noexcept override;
    [[nodiscard]] FunctionBase* Clone() const noexcept override;
};

/// <summary>
/// Represents an absolute value function (V shape function).
/// </summary>
class MATH_LIB AbsoluteValue : public FunctionBase
{
private:
    void ChildRemoved(FunctionBase* Child) noexcept override;

    FunctionBase* N = nullptr;
public:
    AbsoluteValue(FunctionBase* N, double A);
    AbsoluteValue(const AbsoluteValue& Obj) = delete;
    AbsoluteValue(AbsoluteValue& Obj) = delete;

    AbsoluteValue& operator=(const AbsoluteValue& Obj) = delete;
    AbsoluteValue& operator=(AbsoluteValue&& Obj) = delete;

    [[nodiscard]] [[maybe_unused]] const FunctionBase& Base() const;
    [[nodiscard]] [[maybe_unused]] FunctionBase& Base();
    void Base(FunctionBase* NewN);

    MathVector Evaluate(const MathVector& X, bool& Exists) const noexcept override;

    [[nodiscard]] bool ComparesTo(const FunctionBase* Obj) const noexcept override;
    [[nodiscard]] bool EquatesTo(const FunctionBase* Obj) const noexcept override;
    [[nodiscard]] FunctionBase* Clone() const noexcept override;
};

/// <summary>
/// Represents an exponential function of function base N and power B.
/// </summary>
class MATH_LIB Exponent : public FunctionBase
{
private:
    void ChildRemoved(FunctionBase* Obj) noexcept override;

    FunctionBase* N = nullptr;
public:
    Exponent(FunctionBase* N, double Base, double A);
    Exponent(const Exponent& Obj) = delete;
    Exponent(Exponent&& Obj) = delete;

    Exponent& operator=(const Exponent& Obj) = delete;
    Exponent& operator=(Exponent&& Obj) = delete;

    double Base;
    [[nodiscard]] const FunctionBase& Power() const;
    [[nodiscard]] FunctionBase& Power();
    void Power(FunctionBase* NewFunction);

    MathVector Evaluate(const MathVector& X, bool& Exists) const noexcept override;

    [[nodiscard]] bool ComparesTo(const FunctionBase* Obj) const noexcept override;
    [[nodiscard]] bool EquatesTo(const FunctionBase* Obj) const noexcept override;
    [[nodiscard]] FunctionBase* Clone() const noexcept override;
};

/// <summary>
/// Represents a logarithm of base B, evaluating function N.
/// </summary>
class MATH_LIB Logarithm : public FunctionBase
{
private:
    void ChildRemoved(FunctionBase* Child) noexcept override;

    FunctionBase* N = nullptr;
public:
    Logarithm(FunctionBase* N, double Base, double A);
    Logarithm(const Logarithm& Obj) = delete;
    Logarithm(Logarithm&& Obj) = delete;

    Logarithm& operator=(const Logarithm& Obj) = delete;
    Logarithm& operator=(Logarithm&& Obj) = delete;

    double Base;
    [[nodiscard]] [[maybe_unused]] const FunctionBase& Function() const;
    [[nodiscard]] [[maybe_unused]] FunctionBase& Function();
    void Function(FunctionBase* Obj);

    MathVector Evaluate(const MathVector& X, bool& Exists) const noexcept override;

    [[nodiscard]] bool ComparesTo(const FunctionBase* Obj) const noexcept override;
    [[nodiscard]] bool EquatesTo(const FunctionBase* Obj) const noexcept override;
    [[nodiscard]] FunctionBase* Clone() const noexcept override;
};

enum TrigFunc
{
    Sine [[maybe_unused]] = 0,
    Cosine [[maybe_unused]] = 1,
    Tangent [[maybe_unused]] = 2,
    Inverse [[maybe_unused]] = 4,//Use for arc sine/arc cosecant, arc cosine/arc secant, arc tangent/arc cotangent
    Reciprocal [[maybe_unused]] = 8 //Use for cosecant, secant, cotangent
};

/// <summary>
/// Represents the 3 common trig functions: sine, cosine, and tangent.
/// </summary>
class MATH_LIB Trig : public FunctionBase
{
private:
    void ChildRemoved(FunctionBase* Obj) noexcept override;

    unsigned Type;
    FunctionBase* N = nullptr;
public:
    Trig(FunctionBase* Func, unsigned Type, double A);
    Trig(const Trig& Obj) = delete;
    Trig(Trig&& Obj) = delete;

    Trig& operator=(const Trig& Obj) = delete;
    Trig& operator=(Trig&& Obj) = delete;

    [[nodiscard]] unsigned GetType() const { return Type; }
    [[nodiscard]] [[maybe_unused]] const FunctionBase& Function() const;
    [[nodiscard]] [[maybe_unused]] FunctionBase& Function();
    void Function(FunctionBase* NewObj);

    MathVector Evaluate(const MathVector& X, bool& Exists) const noexcept override;

    [[nodiscard]] bool ComparesTo(const FunctionBase* Obj) const noexcept override;
    [[nodiscard]] bool EquatesTo(const FunctionBase* Obj) const noexcept override;
    [[nodiscard]] FunctionBase* Clone() const noexcept override;
};

#endif //JASON_COREFUNCTIONS_H
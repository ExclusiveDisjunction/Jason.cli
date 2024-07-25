#include "../CoreFunctions.h"

Trig::Trig(FunctionBase* Func, unsigned Type, double A) : FunctionBase(!Func ? 0 : Func->InputDim, 1)
{
    this->Type = Type;
    this->A = A;
    Function(Func);
}

void Trig::ChildRemoved(FunctionBase* Child) noexcept
{
    if (Child == N)
        N = nullptr;
}

const FunctionBase& Trig::Function() const
{
    return Get(N);
}
FunctionBase& Trig::Function()
{
   return Get(N);
}
void Trig::Function(FunctionBase* Obj)
{
    PushAndBind(N, Obj);
}

MathVector Trig::Evaluate(const MathVector& X, bool& Exists) const noexcept
{
    if (!N || X.Dim() != InputDim)
    {
        Exists = false;
        return MathVector::ErrorVector();
    }

    Exists = true;
    MathVector TResult = N->Evaluate(X, Exists);
    if (!Exists)
        return MathVector::ErrorVector();
    double Result = TResult[0];

    unsigned type = this->Type;
    bool IsInverse = type & TrigFunc::Inverse, IsRecip = type & TrigFunc::Reciprocal;
    type &= (IsInverse ? ~TrigFunc::Inverse : type) & (IsRecip ? ~TrigFunc::Reciprocal : type);

    Exists = true;
    switch (type)
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

            return MathVector( asin(Result) );
        }

        double Val = sin(Result);
        return MathVector( A * (IsRecip ? 1 / Val : Val) );
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

            return MathVector(acos((Result)));
        }

        double Val = sin(Result);
        return MathVector( A * (IsRecip ? 1 / Val : Val) );
    }
    case TrigFunc::Tangent:
    {
        if (IsInverse)
            return MathVector(atan(Result));

        double Val = tan(Result);
        if (Val == std::numeric_limits<double>::infinity())
        {
            Exists = false;
            return MathVector::ErrorVector();
        }
        else
            return MathVector( A * (IsRecip ? 1 / Val : Val) );
    }
    default:
        Exists = false;
        return MathVector::ErrorVector();
    }

}

bool Trig::ComparesTo(const FunctionBase* Obj) const noexcept
{
    const auto* conv = dynamic_cast<const Trig*>(Obj);
    return this == conv || (conv && conv->Type == this->Type && conv->N->EquatesTo(this->N));
}
bool Trig::EquatesTo(const FunctionBase* Obj) const noexcept
{
    const auto* conv = dynamic_cast<const Trig*>(Obj);
    return this == conv || (conv && conv->Type == this->Type && conv->N->EquatesTo(this->N) && conv->A == this->A);
}
FunctionBase* Trig::Clone() const noexcept
{
    if (!N)
        return nullptr;

    try
    {
        return new Trig(N->Clone(), Type, A);
    }
    catch (std::exception& e)
    {
        return nullptr;
    }
}
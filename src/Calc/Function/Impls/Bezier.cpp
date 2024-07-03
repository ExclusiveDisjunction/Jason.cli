#include "Bezier.h"

BezierMonomial::BezierMonomial(unsigned Dim, unsigned i, unsigned n, const MathVector& Target) : FunctionBase(1, Dim), Point(Target)
{
    if (Target.Dim() != Dim)
        throw std::logic_error("The input dimensions does not match the vector's input dimensions.");

    this->i = i;
    this->n = n;
    float fac = 1.0f;
    for (int j = 1; j <= i; j++)
        fac *= j;
    this->A = (pow(n, i)) / (fac);
}

MathVector BezierMonomial::Evaluate(const MathVector& T, bool& Exists) const noexcept
{
    if (T.Dim() != 1 || T[0] < 0 || T[1] > 1) //Out of range
    {
        Exists = false;
        return MathVector::ErrorVector();
    }

    return Point * (this->A * pow(1 - T[0], n - 1) * pow(T[0], i));
}

[[nodiscard]] bool BezierMonomial::ComparesTo(const FunctionBase* obj) const noexcept
{
    const auto* conv = dynamic_cast<const BezierMonomial*>(obj);
    return conv == this || (conv && conv->n == this->n && conv->i && this->i && conv->Point == this->Point);
}
[[nodiscard]] bool BezierMonomial::EquatesTo(const FunctionBase* obj) const noexcept
{
    const auto* conv = dynamic_cast<const BezierMonomial*>(obj);
    return conv == this || (conv && conv->n == this->n && conv->i && this->i && conv->Point == this->Point && conv->A == this->A);
}
FunctionBase* BezierMonomial::Clone() const noexcept
{
    auto* Return = new BezierMonomial(this->OutputDim, i, n, this->Point);
    Return->A = this->A;
    return Return;
}

[[nodiscard]] [[maybe_unused]] Polynomial* CreateBezier(unsigned Dim, unsigned Rank, const std::vector<MathVector>& Points)
{
    if (Dim == 0 || Rank == 0)
        throw std::logic_error("Dim nor Rank can be zero.");
    else if (Points.size() != Rank + 1)
        throw std::logic_error("Not enough points, or too many points, to match the rank (Expects Rank + 1 points)");

    auto* Return = new Polynomial(1, Dim);
    for (unsigned i = 0; i < Rank + 1; i++)
    {
        const MathVector& curr = Points[i];
        if (curr.Dim() != Dim)
            throw std::logic_error("Cannot accept vector, because of dimension mismatch.");

        Return->AddFunction(new BezierMonomial(Dim, i, Rank, curr));
    }

    return Return;
}
#pragma once

#include "../../Calc/StdCalc.h"
#include "../FunctionBase.h"
#include "../Composite/Polynomial.h"

class MATH_LIB BezierMonomial : public FunctionBase
{
private:
    void ChildRemoved(FunctionBase* Child) noexcept override {}

    MathVector Point;
    unsigned i = 0;
    unsigned n = 0;

public:
    BezierMonomial(unsigned int Dim, unsigned i, unsigned n, const MathVector& Target);

    MathVector Evaluate(const MathVector& T, bool& Exists) const noexcept override;

    [[nodiscard]] bool ComparesTo(const FunctionBase* obj) const noexcept override;
    [[nodiscard]] bool EquatesTo(const FunctionBase* obj) const noexcept override;
    [[nodiscard]] FunctionBase* Clone() const noexcept override;
};

[[nodiscard]] [[maybe_unused]] Polynomial* CreateBezier(unsigned Dim, unsigned Rank, const std::vector<MathVector>& Points);
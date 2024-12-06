//
// Created by Hollan on 7/27/24.
//

#ifndef JASON_EXPRESSIONELEMENT_H
#define JASON_EXPRESSIONELEMENT_H

#include <iostream>
#include <memory>

enum ExpressionElementT
{
    SubExpr,
    Operator
};

class ExpressionElement
{
public:
    virtual ~ExpressionElement() = default;

    virtual void Print(std::ostream& out) const noexcept = 0;
    [[nodiscard]] virtual ExpressionElementT ElementType() const noexcept = 0;
    [[nodiscard]] virtual std::unique_ptr<ExpressionElement> Clone() const noexcept = 0;
};

inline std::ostream& operator<<(std::ostream& out, const ExpressionElement& e)
{
    e.Print(out);
    return out;
}


#endif //JASON_EXPRESSIONELEMENT_H

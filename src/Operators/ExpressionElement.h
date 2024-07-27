//
// Created by Hollan on 7/27/24.
//

#ifndef JASON_EXPRESSIONELEMENT_H
#define JASON_EXPRESSIONELEMENT_H

#include <string>

#include "../Calc/VariableType.h"
#include "ExpressionType.h"

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
    [[nodiscard]] constexpr virtual ExpressionElementT GetType() const noexcept = 0;
};

class SubExpression : public ExpressionElement
{
private:
    ExpressionType* Included = nullptr;
public:
    explicit SubExpression(ExpressionType* Included);
    SubExpression(const ExpressionElement& Obj) = delete;
    SubExpression(SubExpression&& Obj) = delete;

    SubExpression& operator=(const SubExpression& Obj) = delete;
    SubExpression& operator=(SubExpression&& Obj) = delete;

    [[nodiscard]] const ExpressionType& Data() const;
    [[nodiscard]] ExpressionType& Data();

    void Print(std::ostream& out) const noexcept override;
    [[nodiscard]] constexpr virtual ExpressionElementT GetType() const noexcept override
    {
        return ExpressionElementT::SubExpr;
    }
};

std::ostream& operator<<(std::ostream& out, const ExpressionElement& e);


#endif //JASON_EXPRESSIONELEMENT_H

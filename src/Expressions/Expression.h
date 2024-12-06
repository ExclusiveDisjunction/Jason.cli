//
// Created by exdisj on 7/18/24.
//

#ifndef JASON_EXPRESSION_H
#define JASON_EXPRESSION_H

#include <string>

#include "SubExpression.h"
#include "Operator.h"
#include "../Calc/VariableType.h"

class DelimitedExpression
{
private:
    DelimitedExpression(std::vector<std::unique_ptr<ExpressionElement>> elements) : elements(std::move(elements)) { }

    std::vector<std::unique_ptr<ExpressionElement>> elements;

public:
    [[nodiscard]] DelimitedExpression Parse(std::istream& in);

    friend class Expression;
};

class Expression
{
private:
    std::vector<std::unique_ptr<ExpressionElement>> elements;

    Expression() = default;
    explicit Expression(std::vector<std::unique_ptr<ExpressionElement>> list) : elements(std::move(list)) {}
public:
    Expression(const Expression& obj) noexcept
    {
        this->elements.resize(obj.elements.size());

        auto curr = this->elements.begin(), end = this->elements.end();
        auto currO = obj.elements.begin(), endO = obj.elements.end();
        for (; curr != end && currO != endO; curr++, currO++)
            *curr = (*currO)->Clone();
    }
    Expression(Expression&& obj) noexcept : elements(std::move(obj.elements))
    {

    }

    Expression& operator=(const Expression& obj) noexcept
    {
        this->elements.clear();
        this->elements.resize(obj.elements.size());

        auto curr = this->elements.begin(), end = this->elements.end();
        auto currO = obj.elements.begin(), endO = obj.elements.end();
        for (; curr != end && currO != endO; curr++, currO++)
            *curr = (*currO)->Clone();

        return *this;
    }
    Expression& operator=(Expression&& obj) noexcept
    {
        this->elements = std::move(obj.elements);
        return *this;
    }

    [[nodiscard]] static Expression Parse(DelimitedExpression&& delExpr);

    std::unique_ptr<VariableType> Compute(class Session& on) const;
};

std::ostream& operator<<(std::ostream& out, const Expression& e);


#endif //JASON_EXPRESSION_H
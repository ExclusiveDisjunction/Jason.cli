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
public:
    [[nodiscard]] static Expression Parse(DelimitedExpression&& delExpr);

    std::unique_ptr<VariableType> Compute(class Session& on) const;
};

std::ostream& operator<<(std::ostream& out, const Expression& e);


#endif //JASON_EXPRESSION_H
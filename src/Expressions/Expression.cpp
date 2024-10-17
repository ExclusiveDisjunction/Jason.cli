//
// Created by exdisj on 7/18/24.
//

#include "Expression.h"
#include <memory>
#include <stack>

#include "../IO/Session.h"

std::unique_ptr<VariableType> Expression::Compute(Session& on) const
{
    std::unique_ptr<VariableType> result;
    if (this->elements.empty())
        result = std::make_unique<Scalar>(0.00f);
    else
    {
        std::stack<std::unique_ptr<VariableType>> work;
        for (const auto& elem: this->elements)
        {
            switch (elem->ElementType())
            {
                case Operator:
                {
                    const auto& oper = dynamic_cast<const class Operator&>(*elem);

                    if (work.size() < 2) //Not enough
                        throw std::logic_error("Format error: Not enough operands");

                    auto b = std::move(work.top());
                    work.pop();
                    auto a = std::move(work.top());
                    work.pop();

                    auto operResult = oper.Evaluate(*a, *b);
                    work.emplace(std::move(operResult));
                    break;
                }
                case SubExpr:
                {
                    const auto& subExpr = dynamic_cast<const class SubExpression&>(*elem);
                    work.push(subExpr.GetValue(on).Clone());
                    break;
                }
            }
        }

        if (work.size() != 1)
            throw std::logic_error("Expression error: The expression does not evaluate down to one result");
        else
        {
            result = std::move(work.top());
            work.pop();
        }
    }

    return result;
}
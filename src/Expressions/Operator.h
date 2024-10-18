//
// Created by exdisj on 7/18/24.
//

#ifndef JASON_OPERATOR_H
#define JASON_OPERATOR_H

#include "ExpressionElement.h"

#include "../Calc/VariableType.h"

#include <functional>
#include <utility>

using OperatorFunc = std::function<std::unique_ptr<VariableType>(const VariableType&, const VariableType&)>;

class Operator : public ExpressionElement
{
private:
    char symbol = 0;
    unsigned precedence = 0;
    OperatorFunc eval;

public:
    Operator(char symbol, unsigned precedence, OperatorFunc eval);
    Operator(const Operator& obj) = default;
    Operator(Operator&& obj) noexcept = default;

    Operator& operator=(const Operator& obj) = default;
    Operator& operator=(Operator&& obj) = default;

    [[nodiscard]] std::unique_ptr<VariableType> Evaluate(const VariableType& a, const VariableType& b) const;

    [[nodiscard]] constexpr unsigned GetPrecedence() const noexcept;

    bool operator==(const Operator& obj) const noexcept;
    bool operator!=(const Operator& obj) const noexcept;

    bool operator<(const Operator& obj) const noexcept;
    bool operator<=(const Operator& obj) const noexcept;
    bool operator>(const Operator& obj) const noexcept;
    bool operator>=(const Operator& obj) const noexcept;

    void Print(std::ostream& out) const noexcept override;
    [[nodiscard]] std::unique_ptr<ExpressionElement> Clone() const noexcept override
    {
        return std::make_unique<Operator>(*this);
    }
    [[nodiscard]] ExpressionElementT ElementType() const noexcept override
    {
        return ExpressionElementT::Operator;
    }
};


#endif //JASON_OPERATOR_H

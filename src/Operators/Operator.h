//
// Created by exdisj on 7/18/24.
//

#ifndef JASON_OPERATOR_H
#define JASON_OPERATOR_H

#include "Expression.h"

#include <functional>
#include <utility>

using OperatorFunc = std::function<VariableType*(const VariableType&, const VariableType&)>;

class Operator : public ExpressionElement
{
private:
    char symbol = 0;
    unsigned precedence = 0;
    OperatorFunc eval;

public:
    Operator(char symbol, unsigned precedence, OperatorFunc eval) : symbol(symbol), precedence(precedence), eval(std::move(eval)) {}
    Operator(const Operator& obj) = delete;
    Operator(Operator&& obj) noexcept = delete;

    Operator& operator=(const Operator& obj) = delete;
    Operator& operator=(Operator&& obj) = delete;

    [[nodiscard]] VariableType* Evaluate() const;

    [[nodiscard]] constexpr unsigned GetPrecedence() const noexcept;

    bool operator==(const Operator& obj) const noexcept;
    bool operator!=(const Operator& obj) const noexcept;

    bool operator<(const Operator& obj) const noexcept;
    bool operator<=(const Operator& obj) const noexcept;
    bool operator>(const Operator& obj) const noexcept;
    bool operator>=(const Operator& obj) const noexcept;

    void Print();
};


#endif //JASON_OPERATOR_H

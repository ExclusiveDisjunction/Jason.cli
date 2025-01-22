//
// Created by exdisj on 7/18/24.
//

#include "Operator.h"

Operator::Operator(char symbol, unsigned precedence, OperatorFunc eval) : symbol(symbol), precedence(precedence), eval(std::move(eval))
{

}

[[nodiscard]] std::unique_ptr<VariableType> Operator::Evaluate(const VariableType& a, const VariableType& b) const
{
    return this->eval(a, b);
}
[[nodiscard]] constexpr unsigned Operator::GetPrecedence() const noexcept
{
    return this->precedence;
}

bool Operator::operator==(const Operator& obj) const noexcept
{
    return this->symbol == obj.symbol && this->precedence == obj.precedence;
}
bool Operator::operator!=(const Operator& obj) const noexcept
{
    return this->symbol != obj.symbol || this->precedence != obj.precedence;
}

bool Operator::operator<(const Operator& obj) const noexcept
{
    return this->precedence < obj.precedence;
}
bool Operator::operator<=(const Operator& obj) const noexcept
{
    return this->precedence <= obj.precedence;
}
bool Operator::operator>(const Operator& obj) const noexcept
{
    return this->precedence > obj.precedence;
}
bool Operator::operator>=(const Operator& obj) const noexcept
{
    return this->precedence >= obj.precedence;
}

void Operator::Print(std::ostream& out) const noexcept
{
    out << this->symbol;
}

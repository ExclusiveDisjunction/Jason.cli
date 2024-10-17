//
// Created by Hollan on 7/27/24.
//

#include "SubExpression.h"

SubExpression::SubExpression(ExpressionType* Included) : Included(Included)
{

}

const ExpressionType& SubExpression::Data() const
{
    if (!Included)
        throw std::logic_error("Cannot dereference a nullptr");

    return *this->Included;
}
ExpressionType& SubExpression::Data()
{
    return const_cast<ExpressionType&>(const_cast<const SubExpression*>(this)->Data());
}

void SubExpression::Print(std::ostream& out) const noexcept
{
    if (!this->Included)
        out << "[Empty Expression]";
    else
        this->Included->Print(out);
}

ParsedSubExpression::ParsedSubExpression(std::unique_ptr<SubExpression*>&& parsed) : parsed(std::move(parsed)), invalid()
{

}
ParsedSubExpression::ParsedSubExpression(InvalidExpr&& invalid) : parsed(), invalid(std::move(invalid))
{

}

ParsedSubExpression ParsedSubExpression::FromParsed(std::unique_ptr<SubExpression*> parsed) noexcept
{
    if (!parsed)
        return ParsedSubExpression { InvalidExpr("No sub-expression was provided") };

    return ParsedSubExpression { std::move(parsed) };
}
ParsedSubExpression ParsedSubExpression::FromError(InvalidExpr&& expr) noexcept
{
    return ParsedSubExpression { std::move(expr) };
}

ParsedSubExpression ParseExpression(const std::string& input)
{

}

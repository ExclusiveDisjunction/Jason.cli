//
// Created by Hollan on 7/27/24.
//

#include "SubExpression.h"

#include "../IO/Session.h"

ParsedSubExpression::ParsedSubExpression(std::unique_ptr<SubExpression>&& parsed) : parsed(std::move(parsed)), invalid()
{

}
ParsedSubExpression::ParsedSubExpression(InvalidExpr&& invalid) : parsed(), invalid(std::move(invalid))
{

}

ParsedSubExpression ParsedSubExpression::FromParsed(std::unique_ptr<SubExpression> parsed) noexcept
{
    if (!parsed)
        return ParsedSubExpression { InvalidExpr("No sub-expression was provided") };

    return ParsedSubExpression { std::move(parsed) };
}
ParsedSubExpression ParsedSubExpression::FromError(InvalidExpr&& expr) noexcept
{
    return ParsedSubExpression { std::move(expr) };
}

bool NumericExpr::IsNumericalString(std::istream& in) noexcept
{

}
bool VariableExpr::IsVariableString(std::istream& in) noexcept
{

}
std::optional<DeclarationExpr::DeclarationSchema> DeclarationExpr::IsDeclarationExpr(std::istream& in) noexcept
{

}

ParsedSubExpression SubExpression::Parse(std::istream& in, const Session& session) noexcept
{

}

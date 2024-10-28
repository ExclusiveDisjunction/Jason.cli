//
// Created by Hollan on 10/28/24.
//

#include "Expression.h"

DelimitedExpression::DelimitedExpression(std::vector<std::unique_ptr<ExpressionElement>> elements) : elements(std::move(elements))
{
    
}

DelimitedExpression DelimitedExpression::Parse(std::istream& in)
{
    
}

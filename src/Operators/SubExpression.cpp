//
// Created by Hollan on 7/27/24.
//

#include "ExpressionElement.h"

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
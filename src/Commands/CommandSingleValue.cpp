//
// Created by exdisj on 10/11/24.
//

#include "CommandValue.h"

CommandSingleValue::CommandSingleValue(const CommandSingleValue& obj) noexcept : Value(obj.Value)
{

}
CommandSingleValue::CommandSingleValue(CommandSingleValue&& obj) noexcept : Value(std::move(obj.Value))
{

}

CommandSingleValue& CommandSingleValue::operator=(const CommandSingleValue& obj) noexcept
{
    if (this == &obj)
        return *this;

    this->Value = obj.Value;
    return *this;
}
CommandSingleValue& CommandSingleValue::operator=(CommandSingleValue&& obj) noexcept
{
    this->Value = std::move(obj.Value);
    return *this;
}

CommandSingleValue CommandSingleValue::Parse(std::istream& in)
{
    CommandSingleValue result;
    in >> result.Value;

    return result;
}
CommandValue* CommandSingleValue::Clone() const noexcept
{
    return new CommandSingleValue(*this);
}

void CommandSingleValue::Print(std::ostream& out) const noexcept
{
    out << this->Value;
}

std::istream& operator>>(std::istream& in, CommandSingleValue& obj)
{
    obj = CommandSingleValue::Parse(in);
    return in;
}
//
// Created by Hollan on 10/6/24.
//

#include "CommandValue.h"

CommandValue::CommandValue(const CommandValue& obj) noexcept : Value(obj.Value)
{

}
CommandValue::CommandValue(CommandValue&& obj) noexcept : Value(std::move(obj.Value))
{

}

CommandValue& CommandValue::operator=(const CommandValue& obj) noexcept
{
    if (this == &obj)
        return *this;

    this->Value = obj.Value;
    return *this;
}
CommandValue& CommandValue::operator=(CommandValue&& obj) noexcept
{
    this->Value = std::move(obj.Value);
    return *this;
}

CommandValue CommandValue::Parse(std::istream& in)
{
    CommandValue result;
    in >> result.Value;

    return result;
}

void CommandValue::Print(std::ostream& out) const noexcept
{
    out << this->Value;
}

std::ostream& operator<<(std::ostream& out, const CommandValue& obj) noexcept
{
    obj.Print(out);
    return out;
}
std::istream& operator>>(std::istream& in, CommandValue& obj)
{
    obj = CommandValue::Parse(in);
    return in;
}
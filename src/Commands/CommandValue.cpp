//
// Created by Hollan on 10/6/24.
//

#include "CommandParser.h"

const char CommandMultiValue::DenoteStart = '[';
const char CommandMultiValue::DenoteEnd = ']';

CommandValue* CommandValue::Parse(std::istream& in)
{
    auto pos = in.tellg();
    char beg;
    in >> beg;

    if (beg == CommandMultiValue::DenoteStart)
    {
        in.seekg(beg);
        return new CommandMultiValue(std::move(CommandMultiValue::Parse(in)));
    }
    else
        return new CommandSingleValue(std::move(CommandSingleValue::Parse(in)));
}

CommandSingleValue::CommandSingleValue(const CommandSingleValue& obj) noexcept : this->Value(obj.Value)
{

}
CommandSingleValue::CommandSingleValue(CommandSingleValue&& obj) noexcept : this->Value(std::move(obj.Value))
{

}

CommandSingleValue CommandSingleValue::Parse(std::istream& in)
{
    CommandSingleValue result;
    std::getline(in, result.Value);

    return result;
}
CommandMultiValue CommandMultiValue::Parse(std::istream& in)
{
    if (in.bad())
        throw std::logic_error("Argument error, in is not valid.");

    char curr;
    in >> curr;
    if (curr != CommandMultiValue::DenoteStart)
        throw std::logic_error(std::string("Invalid format, multi value not started with '") + CommandMultiValue::DenoteStart + "\'.");

    CommandMultiValue result;
    auto& list = result.Value;
    std::string currStr;
    while (curr != CommandMultiValue::DenoteEnd && !in.eof())
    {
        in >> curr;
        switch (curr)
        {
        case ']':
            break;
        case '[':
            throw std::logic_error("Format error: Start denoted after a start has been declared.");
        case ',':
        {
            list.emplace_back(std::move(currStr));
            currStr.clear();
            continue;
        }
        default:
            currStr += curr;
            break;
        }
    }

    list.emplace_back(std::move(currStr));
}

std::ostream& operator<<(std::ostream& out, const CommandValue& obj) noexcept
{
    obj.Print(out);
    return out;
}
std::istream& operator>>(std::istream& in, CommandSingleValue& obj)
{
    obj = CommandSingleValue::Parse(in);
    return in;
}
std::istream& operator>>(std::istream& in, CommandMultiValue& obj)
{
    obj = CommandMultiValue::Parse(in);
    return in;
}
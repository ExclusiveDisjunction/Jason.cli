#include "CommandSpecifier.h"
#include "../Core/Common.h"

#include <sstream>
#include <algorithm>

CommandSpecifier::CommandSpecifier() : Name(), Value()
{

}
CommandSpecifier::CommandSpecifier(std::string name) : Name(std::move(name))
{

}
CommandSpecifier::CommandSpecifier(std::string name, CommandValue&& obj) : Name(std::move(name)), Value(std::move(obj))
{

}
CommandSpecifier::CommandSpecifier(const CommandSpecifier& obj) noexcept = default;
CommandSpecifier::CommandSpecifier(CommandSpecifier&& obj) noexcept : Name(std::move(obj.Name)), Value(std::move(obj.Value))
{

}
CommandSpecifier::~CommandSpecifier()
{

}

CommandSpecifier& CommandSpecifier::operator=(const CommandSpecifier& obj) noexcept
{
    if (this == &obj)
        return *this;

    this->Name = obj.Name;
    this->Value = obj.Value;
    return *this;
}
CommandSpecifier& CommandSpecifier::operator=(CommandSpecifier&& obj) noexcept
{
    this->Name = std::move(obj.Name);
    this->Value = std::move(obj.Value);

    return *this;
}

CommandSpecifier CommandSpecifier::Parse(std::istream& in)
{
    char one, two;
    in >> one >> two;
    if (one != '-' && two != '-') //Not --
        throw std::logic_error("Format error: Could not construct a command specifier from this input.");

    std::string nameRaw;
    in >> nameRaw;
    auto iter = std::find(nameRaw.begin(), nameRaw.end(), '='); //Looking for a value.
    CommandSpecifier result;

    if (iter == nameRaw.end())
        result.Name = nameRaw;
    else
    {
        size_t loc = std::distance(nameRaw.begin(), iter);
        result.Name = nameRaw.substr(0, loc);
        std::string value_str = nameRaw.substr(loc+1);
        if (value_str.empty())
            throw std::logic_error("Format Error: Empty specifier value");

        std::stringstream value_stream(value_str);
        result.Value = CommandValue::Parse(value_stream);
    }

    return result;
}

std::ostream& operator<<(std::ostream& out, const CommandSpecifier& obj) noexcept
{
    obj.Print(out);
    return out;
}
std::istream& operator>>(std::istream& in, CommandSpecifier& obj)
{
    obj = CommandSpecifier::Parse(in);
    return in;
}

void CommandSpecifier::Print(std::ostream& out) const noexcept
{
    out << "--" << Name;
    if (Value.has_value())
        out << '=' << *Value;
}
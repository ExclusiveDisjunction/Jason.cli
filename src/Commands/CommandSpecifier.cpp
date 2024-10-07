#include "CommandParser.h"

#include "../Common.h"

#include <sstream>

CommandSpecifier::CommandSpecifier(std::string name, std::optional<std::string> value) : Name(std::move(name)), Value(std::move(value))
{

}
CommandSpecifier::CommandSpecifier(const CommandSpecifier& obj) noexcept = default;
CommandSpecifier::CommandSpecifier(CommandSpecifier&& obj) noexcept = default;

CommandSpecifier& CommandSpecifier::operator=(const CommandSpecifier& obj) noexcept = default;
CommandSpecifier& CommandSpecifier::operator=(CommandSpecifier&& obj) noexcept = default;

CommandSpecifier CommandSpecifier::Parse(std::istream& in)
{
    char one, two;
    in >> one >> two;
    if (one != '-' && two != '-') //Not --
        throw std::logic_error("Format error: Could not construct a command specifier from this input.");

    CommandSpecifier result;
    std::string& name = result.Name;
    while (one != '=' && !in.eof())
    {
        in >> one;
        if (one == '=')
            break;
        else
            name += one;
    }
    trim(name);

    if (one == '=') //We get the value
    {
        result.Value = std::string();
        std::string& value = *result.Value;

        std::getline(in, value);
        trim(value);
    }
}
CommandSpecifier CommandSpecifier::Parse(const std::string& in)
{
    std::stringstream ss(in);
    return Parse(ss);
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
//
// Created by Hollan on 10/4/24.
//

#include "CommandParser.h"

CommandParser::CommandParser(std::string name, std::vector<char> flags, std::vector<CommandSpecifier> specifiers, std::vector<CommandValue*> values) :
top_command(std::move(name)), flags(std::move(flags)), specifiers(std::move(specifiers)), values(std::move(values))
{

}

CommandParser CommandParser::Parse(std::istream& in)
{

}

void CommandParser::Print(std::ostream& out) const noexcept
{
    out << top_command << ' ';
    if (!flags.empty())
    {
        out << '-';
        for (const auto& flag : flags)
            out << flag;
        out << ' ';
    }

    for (const auto& specifier : specifiers)
        out << specifier << ' ';

    for (const auto& value : values)
        out << *value << ' ';
}

std::ostream& operator<<(std::ostream& out, const CommandParser& obj) noexcept
{
    obj.Print(out);
    return out;
}
std::istream& operator>>(std::istream& in, CommandParser& obj)
{
    obj = CommandParser::Parse(in);
    return in;
}

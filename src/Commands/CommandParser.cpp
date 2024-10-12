//
// Created by Hollan on 10/4/24.
//

#include "CommandParser.h"

#include <sstream>

CommandParser::CommandParser() = default;
CommandParser::CommandParser(std::string name, std::vector<char> flags, std::vector<CommandSpecifier> specifiers, std::vector<CommandValue*> values) :
top_command(std::move(name)), flags(std::move(flags)), specifiers(std::move(specifiers)), values(std::move(values))
{

}
CommandParser::~CommandParser()
{
    for (auto& val : values)
    {
        delete val;
        val = nullptr;
    }

    values.clear();
}

const std::string& CommandParser::TopCommand() const noexcept
{
    return this->top_command;
}
const std::vector<char>& CommandParser::Flags() const noexcept
{
    return this->flags;
}
const std::vector<CommandSpecifier>& CommandParser::Specifiers() const noexcept
{
    return this->specifiers;
}
const std::vector<CommandValue*>& CommandParser::Values() const noexcept
{
    return this->values;
}

CommandParser CommandParser::Parse(std::istream& in)
{
    CommandParser result;

    in >> result.top_command;
    std::string wholeLineR;
    std::getline(in, wholeLineR);
    std::stringstream wholeLine(wholeLineR);
    wholeLineR.clear();

    while (!wholeLine.eof())
    {
        std::string element;
        wholeLine >> element;
        std::stringstream elementS(element);
        if (element.empty())
            continue;

        if (element.starts_with("--")) //Specifier
            result.specifiers.emplace_back(std::move(CommandSpecifier::Parse(elementS)));
        else if (element.starts_with("-")) //Flag(s)
        {
            char f;
            elementS >> f; //Remove the '-'

            elementS >> f; //First char
            do
            {
                if (!isalpha(f)) //Format error
                    throw std::logic_error("Format error: Flags can only be alphabetical");

                result.flags.emplace_back(f);
                elementS >> f;
            } while(!elementS.eof());
        }
        else //Value
            result.values.emplace_back(CommandValue::Parse(elementS));

        element.clear();
    }

    return result;
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

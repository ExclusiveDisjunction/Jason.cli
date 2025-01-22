//
// Created by Hollan on 10/4/24.
//

#include "CommandParser.h"

#include <sstream>
#include <utility>
#include <algorithm>

CommandParser::CommandParser() = default;
CommandParser::CommandParser(const CommandParser& obj) : top_command(obj.top_command), flags(obj.flags), specifiers(obj.specifiers)
{
    for (const auto& val : obj.values)
        this->values.emplace_back(val);
}
CommandParser::CommandParser(CommandParser&& obj) noexcept : top_command(std::move(obj.top_command)),
                                                                     flags(std::move(obj.flags)),
                                                                     specifiers(std::move(obj.specifiers)),
                                                                     values(std::move(obj.values))
{

}
CommandParser::~CommandParser() = default;

CommandParser& CommandParser::operator=(const CommandParser& obj)
{
    if (this == &obj)
        return *this;

    this->top_command = obj.top_command;
    this->flags = obj.flags;
    this->specifiers = obj.specifiers;
    for (const auto& val : obj.values)
        this->values.emplace_back(val);

    return *this;
}
CommandParser& CommandParser::operator=(CommandParser&& obj) noexcept
{
    this->top_command = std::move(obj.top_command);
    this->flags = std::move(obj.flags);
    this->specifiers = std::move(obj.specifiers);
    this->values = std::move(obj.values);

    return *this;
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
const std::vector<CommandValue>& CommandParser::Values() const noexcept
{
    return this->values;
}

CommandParser CommandParser::Parse(std::istream& in)
{
    CommandParser result;

    std::string& top_command = result.top_command;
    auto& specifiers = result.specifiers;
    auto& values = result.values;
    auto& flags = result.flags;

    in >> top_command;
    std::string wholeLineR;
    std::getline(in, wholeLineR);
    if (wholeLineR.empty()) //Nothing to do
        return result;

    std::stringstream wholeLine(wholeLineR);
    wholeLineR.clear();

    while (!wholeLine.eof())
    {
        char first, second;
        std::streampos loc = wholeLine.tellg();
        wholeLine >> first >> second;
        if (wholeLine.eof()) //We know that we are EOF, and only the first char is valid.
        {
            values.emplace_back(std::string(1, first));
            break;
        }

        wholeLine.seekg(loc); // Reset to front

        if (first == '-' && second == '-') //Specifier
            specifiers.emplace_back(std::move(CommandSpecifier::Parse(wholeLine)));
        else if (first == '-') //Flag(s)
        {
            char f;
            wholeLine >> f; //Remove the '-'

            std::string flagsString;
            wholeLine >> flagsString; //Pull in all flags

            for (char item : flagsString)
            {
                if (!isalpha(item))
                    throw std::logic_error("Format error: Flags can only be alphabetical");

                flags.emplace_back(item);
            }
        }
        else //Value
            values.emplace_back(CommandValue::Parse(wholeLine));
    }

    //Now we check for duplicate flags
    bool HasDuplicates = false;

    for (size_t i = 0; i < flags.size(); i++)
    {
        for (size_t j = 0; j < flags.size(); j++)
        {
            if (j == i)
                continue;

            if (flags[i] == flags[j])
            {
                HasDuplicates = true;
                break;
            }
        }
    }

    if (!HasDuplicates) //No duplicates found
        return result;
    else
        throw std::logic_error("Format error: Duplicate flags found");
}

void CommandParser::Print(std::ostream& out) const noexcept
{
    out << TopCommand() << ' ';
    if (!Flags().empty())
    {
        out << '-';
        for (const auto& flag : Flags())
            out << flag;
        out << ' ';
    }

    for (const auto& specifier : Specifiers())
        out << specifier << ' ';

    for (const auto& value : Values())
        out << value << ' ';
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

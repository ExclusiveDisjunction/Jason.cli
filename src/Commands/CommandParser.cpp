//
// Created by Hollan on 10/4/24.
//

#include "CommandParser.h"

#include <sstream>
#include <utility>
#include <algorithm>

CommandParserCore::CommandParserCore(const CommandParserCore& obj) : top_command(obj.top_command), flags(obj.flags), specifiers(obj.specifiers), counter(0)
{
    for (const auto& val : obj.values)
        this->values.emplace_back(val->Clone());
}
CommandParserCore::CommandParserCore(CommandParserCore&& obj) noexcept : top_command(std::move(obj.top_command)),
                                                                     flags(std::move(obj.flags)),
                                                                     specifiers(std::move(obj.specifiers)),
                                                                     values(std::move(obj.values)),
                                                                     counter(std::exchange(obj.counter, 0))
{

}
CommandParserCore::~CommandParserCore()
{
    Free();
}

CommandParserCore& CommandParserCore::operator=(const CommandParserCore& obj) noexcept
{
    if (this == &obj)
        return *this;

    Free();

    this->top_command = obj.top_command;
    this->flags = obj.flags;
    this->specifiers = obj.specifiers;
    for (const auto& val : obj.values)
        this->values.emplace_back(val->Clone());

    return *this;
}
CommandParserCore& CommandParserCore::operator=(CommandParserCore&& obj) noexcept
{
    Free();

    this->top_command = std::move(obj.top_command);
    this->flags = std::move(obj.flags);
    this->specifiers = std::move(obj.specifiers);
    this->values = std::move(obj.values);
    this->counter = std::exchange(obj.counter, 0);

    return *this;
}

void CommandParserCore::Increment()
{
    this->counter++;
}
void CommandParserCore::Decrement()
{
    this->counter--;
    if (this->counter <= 0)
        delete this;
}
void CommandParserCore::Free()
{
    for (auto& item : values)
        delete item;
    values.clear();
    specifiers.clear();
    flags.clear();
    top_command.clear();
}

CommandParser::CommandParser() : core(new CommandParserCore())
{
    core->Increment();
}
CommandParser::CommandParser(const CommandParser& obj) : core(obj.core)
{
    core->Increment();
}
CommandParser::CommandParser(CommandParser&& obj) noexcept : core(std::exchange(obj.core, nullptr))
{
    // Counter does not move.
}
CommandParser::~CommandParser()
{
    if (this->core)
        this->core->Decrement();

    this->core = nullptr;
}

CommandParser& CommandParser::operator=(const CommandParser& obj)
{
    if (this == &obj || this->core == obj.core) //Already sharing a core
        return *this;

    if (this->core)
        this->core->Decrement();

    this->core = obj.core;
    this->core->Increment();
    return *this;
}
CommandParser& CommandParser::operator=(CommandParser&& obj) noexcept
{
    if (this->core)
        this->core->Decrement();

    this->core = std::exchange(obj.core, nullptr);
    //No increment

    return *this;
}

const std::string& CommandParser::TopCommand() const noexcept
{
    return this->core->top_command;
}
const std::vector<char>& CommandParser::Flags() const noexcept
{
    return this->core->flags;
}
const std::vector<CommandSpecifier>& CommandParser::Specifiers() const noexcept
{
    return this->core->specifiers;
}
const std::vector<CommandValue*>& CommandParser::Values() const noexcept
{
    return this->core->values;
}

CommandParser CommandParser::Parse(std::istream& in)
{
    CommandParser result;

    std::string& top_command = result.core->top_command;
    auto& specifiers = result.core->specifiers;
    auto& values = result.core->values;
    auto& flags = result.core->flags;

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

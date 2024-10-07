//
// Created by Hollan on 10/6/24.
//

#include "CommandParser.h"

#include "../Common.h"

struct only_end_line_whitespace : std::ctype<char>
{
    static const mask* make_table()
    {
        static mask
                rc[table_size]; // Creating a table with the
        // size of the character set
        rc['\n']
                = ctype_base::space; // Set the newline
        // character to be treated
        // as whitespace
        return rc; // Return the modified table
    }

    only_end_line_whitespace(std::size_t refs = 0) : ctype(make_table(), false, refs) {}
};

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

CommandMultiValue::CommandMultiValue(const CommandMultiValue& obj) noexcept : Value(std::move(obj.Value))
{

}
CommandMultiValue::CommandMultiValue(CommandMultiValue&& obj) noexcept : Value(std::move(obj.Value))
{

}

CommandMultiValue& CommandMultiValue::operator=(const CommandMultiValue& obj) noexcept
{
    if (this == &obj)
        return *this;

    this->Value = obj.Value;
    return *this;
}
CommandMultiValue& CommandMultiValue::operator=(CommandMultiValue&& obj) noexcept
{
    this->Value = std::move(obj.Value);
    return *this;
}

CommandSingleValue CommandSingleValue::Parse(std::istream& in)
{
    CommandSingleValue result;
    std::getline(in, result.Value);
    trim(result.Value);

    return result;
}
CommandMultiValue CommandMultiValue::Parse(std::istream& in)
{
    if (in.bad())
        throw std::logic_error("Argument error, in is not valid.");

    auto old_loc = in.getloc();
    in.imbue(std::locale(old_loc, new only_end_line_whitespace()));

    char curr;
    CommandMultiValue result;
    auto& list = result.Value;
    bool started = false, stopped = false;
    std::string currStr;

    while (!in.eof() && curr != CommandMultiValue::DenoteEnd)
    {
        in >> curr;
        switch (curr)
        {
        case ']':
            if (!started)
                throw std::logic_error("Format error: List not started.");

            if (stopped)
                throw std::logic_error("Format error: List already stopped.");

            stopped = true;
            break;
        case '[':
        {
            if (started)
                throw std::logic_error("Format error: Value list already started, but started again.");
            
            started = true;
            break;
        }
        case ',':
        {
            if (!started)
                throw std::logic_error("Format error: List not started.");
            if (stopped)
                throw std::logic_error("Format error: List already stopped.");

            trim(currStr);
            list.emplace_back(std::move(currStr));
            currStr.clear();
            continue;
        }
        default:
            if (!started)
                throw std::logic_error("Format error: List not started.");
            if (stopped)
                throw std::logic_error("Format error: List already stopped.");

            currStr += curr;
            break;
        }
    }

    if (!started || !stopped)
        throw std::logic_error("Format error: List not stopped");

    trim(currStr);
    list.emplace_back(std::move(currStr));
    in.imbue(old_loc);
    return result;
}

void CommandSingleValue::Print(std::ostream& out) const noexcept
{
    out << this->Value;
}
void CommandMultiValue::Print(std::ostream& out) const noexcept
{
    out << '{';
    for (const auto& item : this->Value)
    {
        auto iter = std::find_if(item.begin(), item.end(), [](char c) -> bool
        {
            return isspace(c);
        });

        if (iter == item.end()) //No white space
            out << ' ' << item;
        else
            out << " '" << item << '\'';
    }
    
    out << " }";
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
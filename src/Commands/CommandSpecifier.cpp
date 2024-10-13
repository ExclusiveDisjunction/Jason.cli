#include "CommandSpecifier.h"
#include "../Common.h"

#include <sstream>

CommandSpecifier::CommandSpecifier() : Name(), val(nullptr)
{

}
CommandSpecifier::CommandSpecifier(std::string name) : Name(std::move(name)), val(nullptr)
{

}
CommandSpecifier::CommandSpecifier(std::string name, CommandSingleValue&& obj) : Name(std::move(name)), val(new CommandSingleValue(std::move(obj)))
{

}
CommandSpecifier::CommandSpecifier(std::string name, CommandMultiValue&& obj) : Name(std::move(name)), val(new CommandMultiValue(std::move(obj)))
{

}
CommandSpecifier::CommandSpecifier(const CommandSpecifier& obj) noexcept = default;
CommandSpecifier::CommandSpecifier(CommandSpecifier&& obj) noexcept = default;

CommandSpecifier& CommandSpecifier::operator=(const CommandSpecifier& obj) noexcept = default;
CommandSpecifier& CommandSpecifier::operator=(CommandSpecifier&& obj) noexcept = default;

const CommandValue& CommandSpecifier::Value() const
{
    if (!HasValue())
        throw std::logic_error("No Data: There is no value.");

    return *this->val;
}
CommandValue& CommandSpecifier::Value()
{
    return const_cast<CommandValue&>(const_cast<const CommandSpecifier*>(this)->Value());
}
bool CommandSpecifier::HasValue() const noexcept
{
    return this->val != nullptr;
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
        result.Name = nameRaw.substr(0, nameRaw.end() - iter + 1);
        std::string value_str = nameRaw.substr(nameRaw.end() - iter + 2);
        std::stringstream value_stream(value_str);
        result.val = CommandValue::Parse(value_stream);
    }

    return result;
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
    if (val)
        out << '=' << *val;
}
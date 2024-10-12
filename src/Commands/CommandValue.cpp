//
// Created by Hollan on 10/6/24.
//

#include "CommandValue.h"

CommandValue* CommandValue::Parse(std::istream& in)
{
    auto pos = in.tellg();
    char beg;
    in >> beg;
    in.seekg(pos);

    if (beg == CommandMultiValue::DenoteStart)
        return new CommandMultiValue(std::move(CommandMultiValue::Parse(in)));
    else
        return new CommandSingleValue(std::move(CommandSingleValue::Parse(in)));
}

std::ostream& operator<<(std::ostream& out, const CommandValue& obj) noexcept
{
    obj.Print(out);
    return out;
}
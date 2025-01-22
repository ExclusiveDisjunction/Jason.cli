//
// Created by exdisj on 10/11/24.
//

#ifndef JASON_COMMANDVALUE_H
#define JASON_COMMANDVALUE_H

#include <iostream>
#include <string>
#include <vector>

class CommandValue;

class CommandValue
{
public:
    CommandValue() = default;
    explicit CommandValue(std::string val) : Value(std::move(val)) {}
    CommandValue(const CommandValue& obj) noexcept;
    CommandValue(CommandValue&& obj) noexcept;

    [[nodiscard]] static CommandValue Parse(std::istream& in);

    CommandValue& operator=(const CommandValue& obj) noexcept;
    CommandValue& operator=(CommandValue&& obj) noexcept;

    std::string Value;

    void Print(std::ostream& out) const noexcept;
};

std::ostream& operator<<(std::ostream& out, const CommandValue& obj) noexcept;
std::istream& operator>>(std::istream& in, CommandValue& obj);

#endif //JASON_COMMANDVALUE_H

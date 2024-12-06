//
// Created by Hollan on 10/4/24.
//

#ifndef JASON_COMMANDPARSER_H
#define JASON_COMMANDPARSER_H

//Represents a collection of commands, arguments, values, and flags.

#include <iostream>
#include <string>
#include <vector>

#include "CommandSpecifier.h"
#include "CommandValue.h"

class CommandParser
{
private:
    std::string top_command;
    std::vector<char> flags;
    std::vector<CommandSpecifier> specifiers;
    std::vector<CommandValue> values;

public:
    CommandParser();
    CommandParser(const CommandParser& obj);
    CommandParser(CommandParser&& obj) noexcept;
    ~CommandParser();

    CommandParser& operator=(const CommandParser& obj);
    CommandParser& operator=(CommandParser&& obj) noexcept;

    [[nodiscard]] const std::string& TopCommand() const noexcept;
    [[nodiscard]] const std::vector<char>& Flags() const noexcept;
    [[nodiscard]] const std::vector<CommandSpecifier>& Specifiers() const noexcept;
    [[nodiscard]] const std::vector<CommandValue>& Values() const noexcept;

    [[nodiscard]] static CommandParser Parse(std::istream& in);

    void Print(std::ostream& out) const noexcept;
};

std::ostream& operator<<(std::ostream& out, const CommandParser& obj) noexcept;
std::istream& operator>>(std::istream& in, CommandParser& obj);

#endif //JASON_COMMANDPARSER_H

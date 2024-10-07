//
// Created by Hollan on 10/4/24.
//

#ifndef JASON_COMMANDPARSER_H
#define JASON_COMMANDPARSER_H

//Represents a collection of commands, arguments, values, and flags.

#include <iostream>
#include <string>
#include <vector>
#include <optional>

class CommandParser;
class CommandSpecifier;
class CommandValue;


class CommandParser
{
private:
    CommandParser(std::string name, std::vector<char> flags, std::vector<CommandSpecifier> specifiers, std::vector<CommandValue*> values);

    std::string top_command;
    std::vector<char> flags;
    std::vector<CommandSpecifier> specifiers;
    std::vector<CommandValue*> values;

public:
    [[nodiscard]] static CommandParser Parse(std::istream& in);

    void Print(std::ostream& out) const noexcept;
};

std::ostream& operator<<(std::ostream& out, const CommandParser& obj) noexcept;
std::istream& operator>>(std::istream& in, CommandParser& obj);

class CommandSpecifier
{
public:
    CommandSpecifier() = default;
    CommandSpecifier(std::string name, std::optional<std::string> value);
    CommandSpecifier(const CommandSpecifier& obj) noexcept;
    CommandSpecifier(CommandSpecifier&& obj) noexcept;

    [[nodiscard]] static CommandSpecifier Parse(std::istream& in);
    [[nodiscard]] static CommandSpecifier Parse(const std::string& in);

    CommandSpecifier& operator=(const CommandSpecifier& obj) noexcept;
    CommandSpecifier& operator=(CommandSpecifier&& obj) noexcept;

    std::string Name;
    std::optional<std::string> Value;

    void Print(std::ostream& out) const noexcept;
};

std::ostream& operator<<(std::ostream& out, const CommandSpecifier& obj) noexcept;
std::istream& operator>>(std::istream& in, CommandSpecifier& obj);

class CommandValue
{
public:
    [[nodiscard]] virtual bool IsSingleValue() const noexcept = 0;

    virtual void Print(std::ostream& out) const noexcept = 0;
    [[nodiscard]] static CommandValue* Parse(std::istream& in);
};

class CommandSingleValue : public CommandValue
{
public:
    CommandSingleValue() = default;
    explicit CommandSingleValue(std::string val) : Value(std::move(val)) {}
    CommandSingleValue(const CommandSingleValue& obj) noexcept;
    CommandSingleValue(CommandSingleValue&& obj) noexcept;

    [[nodiscard]] static CommandSingleValue Parse(std::istream& in);

    CommandSingleValue& operator=(const CommandSingleValue& obj) noexcept;
    CommandSingleValue& operator=(CommandSingleValue&& obj) noexcept;

    std::string Value;

    bool IsSingleValue() const noexcept override { return true; }
    void Print(std::ostream& out) const noexcept override;
};
class CommandMultiValue : public CommandValue
{
public:
    CommandMultiValue() = default;
    explicit CommandMultiValue(std::vector<std::string> list) : Value(std::move(list)) {}
    CommandMultiValue(const CommandMultiValue& obj) noexcept;
    CommandMultiValue(CommandMultiValue&& obj) noexcept;

    CommandMultiValue& operator=(const CommandMultiValue& obj) noexcept;
    CommandMultiValue& operator=(CommandMultiValue&& obj) noexcept;

    static const char DenoteStart, DenoteEnd;
    std::vector<std::string> Value;

    [[nodiscard]] static CommandMultiValue Parse(std::istream& in);

    bool IsSingleValue() const noexcept override { return false; }
    void Print(std::ostream& out) const noexcept override;
};

std::ostream& operator<<(std::ostream& out, const CommandValue& obj) noexcept;
std::istream& operator>>(std::istream& in, CommandSingleValue& obj);
std::istream& operator>>(std::istream& in, CommandMultiValue& obj);


#endif //JASON_COMMANDPARSER_H

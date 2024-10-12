//
// Created by exdisj on 10/11/24.
//

#ifndef JASON_COMMANDVALUE_H
#define JASON_COMMANDVALUE_H

#include <iostream>
#include <string>
#include <vector>

class CommandValue;
class CommandSingleValue;
class CommandMultiValue;

class CommandValue
{
public:
    CommandValue() = default;
    virtual ~CommandValue() = default;

    [[nodiscard]] virtual bool IsSingleValue() const noexcept = 0;

    virtual void Print(std::ostream& out) const noexcept = 0;
    [[nodiscard]] static CommandValue* Parse(std::istream& in);
    [[nodiscard]] virtual CommandValue* Clone() const noexcept = 0;
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

    [[nodiscard]] bool IsSingleValue() const noexcept override { return true; }
    void Print(std::ostream& out) const noexcept override;
    [[nodiscard]] CommandValue* Clone() const noexcept override;
};
class CommandMultiValue : public CommandValue
{
public:
    CommandMultiValue() = default;
    [[maybe_unused]] explicit CommandMultiValue(std::vector<std::string> list) : Value(std::move(list)) {}
    CommandMultiValue(const CommandMultiValue& obj) noexcept;
    CommandMultiValue(CommandMultiValue&& obj) noexcept;

    CommandMultiValue& operator=(const CommandMultiValue& obj) noexcept;
    CommandMultiValue& operator=(CommandMultiValue&& obj) noexcept;

    static const char DenoteStart, DenoteEnd;
    std::vector<std::string> Value;

    [[nodiscard]] static CommandMultiValue Parse(std::istream& in);

    [[nodiscard]] bool IsSingleValue() const noexcept override { return false; }
    void Print(std::ostream& out) const noexcept override;
    [[nodiscard]] CommandValue* Clone() const noexcept override;
};

std::ostream& operator<<(std::ostream& out, const CommandValue& obj) noexcept;
std::istream& operator>>(std::istream& in, CommandSingleValue& obj);
std::istream& operator>>(std::istream& in, CommandMultiValue& obj);

#endif //JASON_COMMANDVALUE_H

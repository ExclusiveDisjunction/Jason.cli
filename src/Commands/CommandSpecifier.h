//
// Created by exdisj on 10/11/24.
//

#ifndef JASON_COMMANDSPECIFIER_H
#define JASON_COMMANDSPECIFIER_H

#include "CommandValue.h"

class CommandSpecifier;

class CommandSpecifier
{
private:
    CommandValue* val;

public:
    CommandSpecifier();
    explicit CommandSpecifier(std::string name);
    CommandSpecifier(std::string name, CommandSingleValue&& obj);
    CommandSpecifier(std::string name, CommandMultiValue&& obj);
    CommandSpecifier(const CommandSpecifier& obj) noexcept;
    CommandSpecifier(CommandSpecifier&& obj) noexcept;

    [[nodiscard]] static CommandSpecifier Parse(std::istream& in);
    [[nodiscard]] static CommandSpecifier Parse(const std::string& in);

    CommandSpecifier& operator=(const CommandSpecifier& obj) noexcept;
    CommandSpecifier& operator=(CommandSpecifier&& obj) noexcept;

    std::string Name;
    [[nodiscard]] const CommandValue& Value() const;
    [[nodiscard]] CommandValue& Value();
    [[nodiscard]] bool HasValue() const noexcept;

    void Print(std::ostream& out) const noexcept;
};

std::ostream& operator<<(std::ostream& out, const CommandSpecifier& obj) noexcept;
std::istream& operator>>(std::istream& in, CommandSpecifier& obj);

#endif //JASON_COMMANDSPECIFIER_H

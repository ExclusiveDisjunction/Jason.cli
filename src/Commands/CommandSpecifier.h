//
// Created by exdisj on 10/11/24.
//

#ifndef JASON_COMMANDSPECIFIER_H
#define JASON_COMMANDSPECIFIER_H

#include "CommandValue.h"

#include <optional>

class CommandSpecifier;

class CommandSpecifier
{
private:
public:
    CommandSpecifier();
    explicit CommandSpecifier(std::string name);
    CommandSpecifier(std::string name, CommandValue&& obj);
    CommandSpecifier(const CommandSpecifier& obj) noexcept;
    CommandSpecifier(CommandSpecifier&& obj) noexcept;
    ~CommandSpecifier();

    [[nodiscard]] static CommandSpecifier Parse(std::istream& in);

    CommandSpecifier& operator=(const CommandSpecifier& obj) noexcept;
    CommandSpecifier& operator=(CommandSpecifier&& obj) noexcept;

    std::string Name;
    std::optional<CommandValue> Value;

    void Print(std::ostream& out) const noexcept;
};

std::ostream& operator<<(std::ostream& out, const CommandSpecifier& obj) noexcept;
std::istream& operator>>(std::istream& in, CommandSpecifier& obj);

#endif //JASON_COMMANDSPECIFIER_H

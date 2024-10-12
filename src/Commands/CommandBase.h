//
// Created by Hollan on 10/4/24.
//

#ifndef JASON_COMMANDBASE_H
#define JASON_COMMANDBASE_H

#include <iostream>
#include <optional>

#include "CommandParser.h"

enum CommandTypeE
{

};
class CommandType
{
public:
    CommandType(CommandTypeE type, std::string name, std::optional<std::string> desc = {}) : EType(type), Name(std::move(name)), Description(std::move(desc))
    {

    }

    const CommandTypeE EType;
    const std::string Name;
    const std::optional<std::string> Description;
};

class Session;


class CommandBase
{
private:

public:
    CommandBase() = default;
    CommandBase(const CommandBase& obj) noexcept = delete;
    CommandBase(CommandBase&& obj) noexcept = default;

    CommandBase& operator=(const CommandBase& obj) noexcept = delete;
    CommandBase& operator=(CommandBase&& obj) noexcept = delete;

    [[nodiscard]] virtual CommandBase* Clone() const noexcept = 0;
    [[nodiscard]] virtual const CommandType& Type() const noexcept = 0;

    template<typename T> requires std::is_base_of<CommandBase, T>::value
    T* Convert() noexcept
    {
        return const_cast<T*>(const_cast<const CommandBase*>(this)->Convert<T>());
    }
    template<typename T> requires std::is_base_of<CommandBase, T>::value
    const T* Convert() const noexcept
    {
        return dynamic_cast<const T*>(this);
    }

    [[nodiscard]] virtual bool Execute(Session& On) = 0;
    [[nodiscard]] virtual bool Parse(const CommandParser& Parser) = 0;
};


#endif //JASON_COMMANDBASE_H

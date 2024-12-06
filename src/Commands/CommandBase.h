//
// Created by Hollan on 10/4/24.
//

#ifndef JASON_COMMANDBASE_H
#define JASON_COMMANDBASE_H

#include <iostream>
#include <optional>
#include <memory>

#include "CommandParser.h"

enum CommandTypeE
{
    C_Evaluate,
    C_Store,

    C_Load,
    C_Unload,
    C_List,
    C_Info,

    C_Export,
    C_Import
};
class CommandType
{
public:
    CommandType(CommandTypeE type, std::string Name, std::string DisplayName, std::optional<std::string> desc = {}) : EType(type), Name(std::move(Name)), DisplayName(std::move(DisplayName)), Description(std::move(desc))
    {

    }

    const CommandTypeE EType;
    const std::string Name, DisplayName;
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

    [[nodiscard]] virtual std::unique_ptr<CommandBase*> Clone() const noexcept = 0;
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

    [[nodiscard]] virtual bool Execute(Session& On, std::ostream& Output) = 0;
    [[nodiscard]] virtual bool Parse(const CommandParser& Parser) = 0;
};


#endif //JASON_COMMANDBASE_H

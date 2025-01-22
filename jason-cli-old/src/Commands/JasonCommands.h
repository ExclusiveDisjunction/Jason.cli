//
// Created by Hollan on 10/15/24.
//

#ifndef JASON_JASONCOMMANDS_H
#define JASON_JASONCOMMANDS_H

#include "CommandBase.h"

#include "../Expressions/Expression.h"
#include "../IO/PackageEntryKey.h"

class EvaluateCommand : public CommandBase
{
private:
    static CommandType ThisType;

public:
    explicit EvaluateCommand(Expression&& Expr);
    explicit EvaluateCommand(const Expression& Expr);

    Expression TargetExpression;
    bool StoreInAns = true;

    [[nodiscard]] std::unique_ptr<CommandBase*> Clone() const noexcept override;
    [[nodiscard]] const CommandType& Type() const noexcept override;

    [[nodiscard]] bool Execute(Session& On, std::ostream& Output) override;
    [[nodiscard]] bool Parse(const CommandParser& Parser) override;
};
class StoreCommand : public CommandBase
{
private:
    static CommandType ThisType;

public:
    StoreCommand(PackageEntryKey Target, Expression&& Expr);

    PackageEntryKey Target;
    Expression TargetExpr;

    [[nodiscard]] std::unique_ptr<CommandBase*> Clone() const noexcept override;
    [[nodiscard]] const CommandType& Type() const noexcept override;

    [[nodiscard]] bool Execute(Session& On, std::ostream& Output) override;
    [[nodiscard]] bool Parse(const CommandParser& Parser) override;
};

class LoadCommand
{

};


#endif //JASON_JASONCOMMANDS_H

//
// Created by Hollan on 10/15/24.
//

#include "../JasonCommands.h"

#include <algorithm>

CommandType EvaluateCommand::ThisType = CommandType(
        CommandTypeE::C_Evaluate,
        "Evaluate",
        "eval",
        "Evaluate an expression, and store the result in ans"
        );

EvaluateCommand::EvaluateCommand(Expression&& Expr) : TargetExpression(std::move(Expr))
{

}
EvaluateCommand::EvaluateCommand(const Expression& Expr) : TargetExpression(Expr)
{

}

std::unique_ptr<CommandBase*> EvaluateCommand::Clone() const noexcept
{
    return std::make_unique<CommandBase*>(new EvaluateCommand(this->TargetExpression));
}
const CommandType& EvaluateCommand::Type() const noexcept
{
    return EvaluateCommand::ThisType;
}

bool EvaluateCommand::Execute(Session& On, std::ostream& Output)
{

}
bool EvaluateCommand::Parse(const CommandParser& Parser)
{
    const auto& flags = Parser.Flags();
    auto flagContained = std::find(flags.begin(), flags.end(), 'n');

    this->StoreInAns = flagContained == flags.end(); //If the flag is not contained, we store in ans.

}
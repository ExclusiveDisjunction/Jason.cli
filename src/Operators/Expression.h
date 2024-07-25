//
// Created by exdisj on 7/18/24.
//

#ifndef JASON_EXPRESSION_H
#define JASON_EXPRESSION_H

#include <string>

#include "../Calc/VariableType.h"

enum ExpressionTypeE
{
    Numeric,
    Variable,
    Declaration,
    Ans,
    Empty,
    Environment,
    Invalid
};


class ExpressionType
{
public:
    virtual ~ExpressionType() = default;

    virtual void Print(std::ostream& out) const noexcept = 0;
    [[nodiscard]] constexpr virtual ExpressionTypeE GetType() const noexcept = 0;

    [[nodiscard]] virtual VariableType& GetValue() const = 0;
};

class NumericExpr : public ExpressionType
{
private:
     VariableType* Value = nullptr;

public:
    NumericExpr(VariableType* Value);
    NumericExpr(const VariableType& Value);
    NumericExpr(const NumericExpr& obj);
    NumericExpr(NumericExpr&& obj) noexcept;
    ~NumericExpr() override;

    NumericExpr& operator=(const NumericExpr& obj);
    NumericExpr& operator=(NumericExpr&& obj);

    [[nodiscard]] constexpr ExpressionTypeE GetType() const noexcept override;
    void Print(std::ostream& out) const noexcept override;

    [[nodiscard]] VariableType& GetValue() const override;
};
class VariableExpr : public ExpressionType
{
private:

public:
    VariableExpr();
    VariableExpr(const VariableExpr& obj);
    VariableExpr(VariableExpr&& obj) noexcept;
    ~VariableExpr() override;

    VariableExpr& operator=(const VariableExpr& obj);
    VariableExpr& operator=(VariableExpr&& obj);

    [[nodiscard]] constexpr ExpressionTypeE GetType() const noexcept override;
    void Print(std::ostream& out) const noexcept override;

    [[nodiscard]] VariableType& GetValue() const override;
};
class DeclarationExpr : public ExpressionType
{

};
class AnsExpr : public ExpressionType
{

};
class EnvironmentExpr : public ExpressionType
{

};
class InvalidExpr : public ExpressionType
{

};

ExpressionType* ParseExpression(const std::string& input);

std::ostream& operator<<(std::ostream& out, const ExpressionType& obj);

enum ExpressionElementT
{
    SubExpr,
    Operator
};

class ExpressionElement
{
public:
    virtual ~ExpressionElement();

    virtual void Print(std::ostream& out) const noexcept = 0;
    [[nodiscard]] constexpr virtual ExpressionElementT GetType() const noexcept = 0;
};

class SubExpression : public ExpressionElement
{

};

std::ostream& operator<<(std::ostream& out, const ExpressionElement& e);

class Expression
{

};

std::ostream& operator<<(std::ostream& out, const Expression& e);


#endif //JASON_EXPRESSION_H

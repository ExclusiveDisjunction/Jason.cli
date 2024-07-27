//
// Created by Hollan on 7/27/24.
//

#ifndef JASON_EXPRESSIONTYPE_H
#define JASON_EXPRESSIONTYPE_H

#include <string>

#include "../Calc/VariableType.h"
#include "../Calc/Numerics/Scalar.h"

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

    [[nodiscard]] virtual const VariableType& GetValue() const = 0;
};

class NumericExpr : public ExpressionType
{
private:
    Scalar Value;

public:
    NumericExpr(Scalar Value);
    NumericExpr(const NumericExpr& obj);
    NumericExpr(NumericExpr&& obj) noexcept;
    ~NumericExpr() override;

    NumericExpr& operator=(const NumericExpr& obj);
    NumericExpr& operator=(NumericExpr&& obj);

    [[nodiscard]] constexpr ExpressionTypeE GetType() const noexcept override;
    void Print(std::ostream& out) const noexcept override;

    [[nodiscard]] const VariableType& GetValue() const override;

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

    [[nodiscard]] const VariableType& GetValue() const override;
};
class DeclarationExpr : public ExpressionType
{
private:
public:
    DeclarationExpr() = default;
    DeclarationExpr(const DeclarationExpr& Obj) = delete;
    DeclarationExpr(DeclarationExpr&& Obj) = delete;

    DeclarationExpr& operator=(const DeclarationExpr& obj) = delete;
    DeclarationExpr& operator=(DeclarationExpr&& obj) = delete;

    [[nodiscard]] constexpr ExpressionTypeE GetType() const noexcept override;
    void Print(std::ostream& out) const noexcept override;

    [[nodiscard]] const VariableType& GetValue() const override;
};
class AnsExpr : public ExpressionType
{
private:
public:
    AnsExpr() = default;
    AnsExpr(const AnsExpr& obj) = delete;
    AnsExpr(AnsExpr&& Obj) = delete;

    AnsExpr& operator=(const AnsExpr& obj) = delete;
    AnsExpr& operator=(AnsExpr&& obj) = delete;

    [[nodiscard]] constexpr ExpressionTypeE GetType() const noexcept override;
    void Print(std::ostream& out) const noexcept override;

    [[nodiscard]] const VariableType& GetValue() const override;
};
class EnvironmentExpr : public ExpressionType
{
private:
public:
    EnvironmentExpr() = default;
    EnvironmentExpr(const EnvironmentExpr& obj) = delete;
    EnvironmentExpr(EnvironmentExpr&& Obj) = delete;

    EnvironmentExpr& operator=(const EnvironmentExpr& obj) = delete;
    EnvironmentExpr& operator=(EnvironmentExpr&& obj) = delete;

    [[nodiscard]] constexpr ExpressionTypeE GetType() const noexcept override;
    void Print(std::ostream& out) const noexcept override;

    [[nodiscard]] const VariableType& GetValue() const override;
};
class InvalidExpr : public ExpressionType
{
private:
    std::string message;

public:
    InvalidExpr(const std::string& m) : message(m) {}
    InvalidExpr(const InvalidExpr& obj) = delete;
    InvalidExpr(InvalidExpr&& Obj) = delete;

    InvalidExpr& operator=(const InvalidExpr& obj) = delete;
    InvalidExpr& operator=(InvalidExpr& obj) = delete;

    [[nodiscard]] const std::string& GetMessage() const noexcept;

    [[nodiscard]] constexpr ExpressionTypeE GetType() const noexcept override;
    void Print(std::ostream& out) const noexcept override;

    [[nodiscard]] const VariableType& GetValue() const override
    {
        std::string Message("Invalid Expression: \" ");
        Message += this->message;
        Message += " \"";

        throw std::logic_error(Message);
    }
};

ExpressionType* ParseExpression(const std::string& input);

std::ostream& operator<<(std::ostream& out, const ExpressionType& obj);

#endif //JASON_EXPRESSIONTYPE_H

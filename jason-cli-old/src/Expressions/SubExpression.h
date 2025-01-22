//
// Created by Hollan on 7/27/24.
//

#ifndef JASON_SUBEXPRESSION_H
#define JASON_SUBEXPRESSION_H

#include <string>
#include <memory>

#include "../Calc/VariableType.h"
#include "../Calc/Numerics/Scalar.h"
#include "../IO/PackageEntryKey.h"
#include "ExpressionElement.h"

class Session;
class ParsedSubExpression;

enum SubExpressionType
{
    NumericExpr,
    VariableExpr,
    DeclarationExpr,
    IntermediateExpr
};

class SubExpression : public ExpressionElement
{
public:
    [[nodiscard]] virtual SubExpressionType GetType() const noexcept = 0;
    [[nodiscard]] ExpressionElementT ElementType() const noexcept override
    {
        return ExpressionElementT::SubExpr;
    }

    [[nodiscard]] virtual const VariableType& GetValue(Session& host) const = 0;

    [[nodiscard]] static ParsedSubExpression Parse(std::istream& in, const Session& session) noexcept;
};

class NumericExpr : public SubExpression
{
private:
    Scalar Value;

public:
    explicit NumericExpr(Scalar Value) : Value(std::move(Value)) { }

    static bool IsNumericalString(std::istream& in) noexcept;

    [[nodiscard]] SubExpressionType GetType() const noexcept override
    {
        return SubExpressionType::NumericExpr;
    }
    void Print(std::ostream& out) const noexcept override
    {
        out << Value;
    }

    [[nodiscard]] const VariableType& GetValue(Session& host) const override
    {
        return Value;
    }

    [[nodiscard]] std::unique_ptr<ExpressionElement> Clone() const noexcept override
    {
        return std::make_unique<NumericExpr>(*this);
    }
};
/// @breif Holds a key to a package entry, but will do nothing after this object goes out of scope.
class VariableExpr : public SubExpression
{
private:
    PackageEntryKey key;

public:
    explicit VariableExpr(PackageEntryKey key);

    [[nodiscard]] static bool IsVariableString(std::istream& in) noexcept; //Assumes the variable is within the session

    [[nodiscard]] SubExpressionType GetType() const noexcept override
    {
        return SubExpressionType::VariableExpr;
    }
    void Print(std::ostream& out) const noexcept override;

    [[nodiscard]] const VariableType& GetValue(Session& host) const override;

    [[nodiscard]] std::unique_ptr<ExpressionElement> Clone() const noexcept override
    {
        return std::make_unique<VariableExpr>(*this);
    }
};
/// @breif Holds a key to a package entry, but will call to delete it after this object goes out of scope.
/// @breif Note that once this object is copied, it will call to copy its object.
class DeclarationExpr : public SubExpression
{
private:
    PackageEntryKey key;

public:
    explicit DeclarationExpr(PackageEntryKey key) : key(key) { }
    DeclarationExpr(const DeclarationExpr& obj);
    DeclarationExpr(DeclarationExpr&& obj) noexcept;
    ~DeclarationExpr() override;

    DeclarationExpr& operator=(const DeclarationExpr& obj);
    DeclarationExpr& operator=(DeclarationExpr&& obj);

    struct DeclarationSchema
    {
        VariableTypes Type;
        int dim1, dim2;
    };
    [[nodiscard]] static std::optional<DeclarationSchema> IsDeclarationExpr(std::istream& in) noexcept;

    [[nodiscard]] SubExpressionType GetType() const noexcept override
    {
        return SubExpressionType::DeclarationExpr;
    }
    void Print(std::ostream& out) const noexcept override;

    [[nodiscard]] std::unique_ptr<ExpressionElement> Clone() const noexcept override
    {
        return std::make_unique<DeclarationExpr>(*this);
    }

    [[nodiscard]] const VariableType& GetValue(Session& host) const override;
};
class IntermediateExpr : public SubExpression
{
private:
    std::string Value;

public:
    explicit IntermediateExpr(std::string Value) : Value(std::move(Value)) { }

    [[nodiscard]] SubExpressionType GetType() const noexcept override
    {
        return SubExpressionType::IntermediateExpr;
    }
    void Print(std::ostream& out) const noexcept override
    {
        out << Value;
    }

    [[nodiscard]] const VariableType& GetValue(Session& host) const override
    {
        throw std::logic_error("Invalid access");
    }
    [[nodiscard]] const std::string& Access() const noexcept
    {
        return Value;
    }

    [[nodiscard]] std::unique_ptr<ExpressionElement> Clone() const noexcept override
    {
        return std::make_unique<IntermediateExpr>(*this);
    }
};

/// @breif Returned when the expression to be parsed is invalid.
class InvalidExpr
{
private:
    std::string message;

public:
    explicit InvalidExpr(std::string m) : message(std::move(m)) {}

    [[nodiscard]] const std::string& GetMessage() const noexcept
    {
        return this->message;
    }
    [[nodiscard]] std::string FormatMessage() const noexcept
    {
        std::string Message("Invalid Expression: \" ");
        Message += this->message;
        Message += " \"";

        return Message;
    }
    void Print(std::ostream& out) const noexcept
    {
        out << this->FormatMessage();
    }
};

struct ParsedSubExpression
{
private:
    std::optional<std::unique_ptr<SubExpression>> parsed;
    std::optional<InvalidExpr> invalid;

    explicit ParsedSubExpression(std::unique_ptr<SubExpression>&& parsed);
    explicit ParsedSubExpression(InvalidExpr&& invalid);

public:
    /// @breif Returns a ParsedSubExpression containing the parsed expression, but if the pointer is nullptr, it will return an error.
    [[nodiscard]] static ParsedSubExpression FromParsed(std::unique_ptr<SubExpression> parsed) noexcept;
    /// @breif Returns a ParsedSubExpression with the corresponding invalid expression.
    [[nodiscard]] static ParsedSubExpression FromError(InvalidExpr&& expr) noexcept;
};

#endif //JASON_SUBEXPRESSION_H

//
// Created by exdisj on 5/26/24.
//

#ifndef JASON_VARIABLETYPE_H
#define JASON_VARIABLETYPE_H

#include <iostream>
#include <sstream>
#include <string>
#include <optional>
#include <memory>

#include "StdCalc.h"

class MATH_LIB VariableType;

enum VariableTypes
{
    VT_Scalar,
    VT_Vector,
    VT_Matrix
};

class MATH_LIB VariableType
{
public:
    virtual ~VariableType() = default;

    [[nodiscard]] virtual VariableTypes GetType() const noexcept = 0;
    [[nodiscard]] virtual std::unique_ptr<VariableType> Clone() const noexcept = 0;

    [[nodiscard]] virtual std::string Sterilize() const noexcept; //Writes to a string value
    virtual void Sterilize(std::ostream& out) const noexcept = 0; //Writes to a file, can be retrieved.
    [[nodiscard]] virtual std::string GetTypeString() const noexcept = 0; //Displays (None), (Scalar), (Vector:D), (Matrix:mxn)
    virtual void Print(std::ostream& out) const noexcept = 0; //Pretty prints

    [[nodiscard]] static std::unique_ptr<VariableType> ApplyOperation(const VariableType& One, const VariableType& Two, char oper);

    virtual bool operator==(const VariableType& obj) const noexcept = 0;
    virtual bool operator!=(const VariableType& obj) const noexcept = 0;

    [[nodiscard]] static std::optional<std::unique_ptr<VariableType>> Desterilize(const std::string& val) noexcept;
    [[nodiscard]] static std::optional<std::unique_ptr<VariableType>> Desterilize(std::istream& in) noexcept;
};

std::ostream& operator<<(std::ostream& out, const VariableType& Obj);

#endif //JASON_VARIABLETYPE_H

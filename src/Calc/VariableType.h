//
// Created by exdisj on 5/26/24.
//

#ifndef JASON_VARIABLETYPE_H
#define JASON_VARIABLETYPE_H

#include <iostream>
#include <sstream>
#include <string>

#include "StdCalc.h"

class MATH_LIB VariableType;

enum VariableTypes
{
    VT_Scalar,
    VT_Complex,
    VT_Vector,
    VT_Matrix
};

class MATH_LIB VariableType
{
public:
    [[nodiscard]] virtual VariableTypes GetType() const noexcept = 0;

    //Moves all information out of the current instance and makes a pointer to the corresponding type. Saves on calling the memory constructor.
    [[nodiscard]] virtual VariableType* MoveIntoPointer() noexcept = 0;

    [[nodiscard]] virtual std::string Sterilize() const noexcept; //Writes to a string value
    virtual void Sterilize(std::ostream& out) const noexcept = 0; //Writes to a file, can be retrieved.
    [[nodiscard]] virtual std::string GetTypeString() const noexcept = 0; //Displays (None), (Scalar), (Vector:D), (Matrix:mxn)
    virtual std::ostream& operator<<(std::ostream& out) const noexcept = 0; //Pretty prints

    [[nodiscard]] static VariableType* ApplyOperation(const VariableType* One, const VariableType* Two, char oper);

    virtual bool operator==(const VariableType& obj) const noexcept = 0;
    virtual bool operator!=(const VariableType& obj) const noexcept = 0;
};

MATH_LIB VariableType* FromSterilized(const std::string& val) noexcept;
MATH_LIB VariableType* FromSterilized(std::istream& in) noexcept;

#endif //JASON_VARIABLETYPE_H

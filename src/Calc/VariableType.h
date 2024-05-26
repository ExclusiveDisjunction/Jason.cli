//
// Created by exdisj on 5/26/24.
//

#ifndef JASON_VARIABLETYPE_H
#define JASON_VARIABLETYPE_H

#include <iostream>
#include <sstream>
#include <string>

enum VariableTypes
{
    VT_None,
    VT_Scalar,
    VT_Vector,
    VT_Matrix
};

class VariableType
{
public:
    VariableType();

    virtual VariableTypes GetType() const = 0;

    virtual std::string Sterilize() const //Writes to a string value
    {
        std::stringstream ss;
        Sterilize(ss);
        return ss.str();
    }
    virtual void Sterilize(std::ostream& out) const = 0; //Writes to a file, can be retrived.
    virtual std::string GetTypeString() const = 0; //Displays (None), (Scalar), (Vector:D), (Matrix:mxn)
    virtual std::ostream& operator<<(std::ostream& out) const = 0; //Pretty prints

    virtual VariableType operator+(const VariableType& in) const = 0;
    virtual VariableType operator-(const VariableType& in) const = 0;
    virtual VariableType operator*(const VariableType& in) const = 0;
    virtual VariableType operator/(const VariableType& in) const = 0;
    virtual VariableType operator%(const VariableType& in) const = 0;
    virtual VariableType pow(const VariableType& in) const = 0;

    virtual bool operator==(const VariableType& obj) const = 0;
    virtual bool operator!=(const VariableType& obj) const = 0;
};

#endif //JASON_VARIABLETYPE_H

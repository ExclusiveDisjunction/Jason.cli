//
// Created by exdisj on 5/26/24.
//

#ifndef JASON_SCALAR_H
#define JASON_SCALAR_H

#include "VariableType.h"

class Scalar : public VariableType
{
public:
    explicit Scalar(double val = 0.00) : Data(val) { }
    explicit Scalar(std::istream& in);

    double Data;

    [[nodiscard]] VariableTypes GetType() const noexcept override
    {
        return VariableTypes::VT_Scalar;
    }
    void Sterilize(std::ostream& out) const noexcept override;
    static Scalar* FromSterilize(const std::string& in)
    {
        try
        {
            std::stringstream ss(in);
            return new Scalar(ss);
        }
        catch (std::logic_error& e)
        {
            throw e;
        }
    }
    [[nodiscard]] std::string GetTypeString() const noexcept override
    {
        return "(Scalar)";
    }

    VariableType* operator+(const VariableType& in) const noexcept override;
    VariableType* operator-(const VariableType& in) const noexcept override;
    VariableType* operator*(const VariableType& in) const noexcept override;
    VariableType* operator/(const VariableType& in) const noexcept override;
    VariableType* operator%(const VariableType& in) const noexcept override;
    [[nodiscard]] VariableType* Pow(const VariableType& in) const noexcept override;

    bool operator==(const VariableType& obj) const noexcept override;
    bool operator!=(const VariableType& obj) const noexcept override;

    std::ostream& operator<<(std::ostream& out) const noexcept override;
};


#endif //JASON_SCALAR_H

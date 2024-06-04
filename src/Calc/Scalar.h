//
// Created by exdisj on 5/26/24.
//

#ifndef JASON_SCALAR_H
#define JASON_SCALAR_H

#include "VariableType.h"
#include "OperatorException.h"

class Scalar : public VariableType
{
public:
    explicit Scalar(double val = 0.00) : Data(val) { }
    explicit Scalar(std::istream& in);

    double Data;

    [[nodiscard]] VariableType* MoveIntoPointer() noexcept override { return new Scalar(*this); }

    [[nodiscard]] VariableTypes GetType() const noexcept override;
    void Sterilize(std::ostream& out) const noexcept override;
    static Scalar* FromSterilize(const std::string& in);
    [[nodiscard]] std::string GetTypeString() const noexcept override;

    Scalar operator+(const Scalar& in) const noexcept;
    Scalar operator+(double in) const noexcept;
    Scalar operator-(const Scalar& in) const noexcept;
    Scalar operator-(double in) const noexcept;
    Scalar operator*(const Scalar& in) const noexcept;
    Scalar operator*(double in) const noexcept;
    Scalar operator/(const Scalar& in) const noexcept;
    Scalar operator/(double in) const noexcept;
    Scalar operator%(const Scalar& in) const;
    Scalar operator%(double in) const;
    [[nodiscard]] Scalar Pow(const Scalar& in) const noexcept;
    [[nodiscard]] Scalar Pow(double in) const noexcept;

    bool operator==(const VariableType& obj) const noexcept override;
    bool operator!=(const VariableType& obj) const noexcept override;

    std::ostream& operator<<(std::ostream& out) const noexcept override;

    constexpr explicit operator double() const noexcept { return Data; }
};


#endif //JASON_SCALAR_H

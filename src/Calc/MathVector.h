#pragma once

#include "StdCalc.h"
#include "VariableType.h"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

class MATH_LIB Matrix;
class MATH_LIB Scalar;

class MATH_LIB MathVector : public VariableType {
private:
    void Allocate(unsigned int Dim, double Val);
    void DeAllocate();

    unsigned int d = 1;
    double *Point = nullptr;

    MathVector();

public:
    explicit MathVector(unsigned int Dim, double Val = 0.0);
    template<std::convertible_to<double>... Args>
    explicit MathVector(Args... Value) noexcept {
        auto ToFill = std::vector<double>({((double) Value)...});
        auto Dim = static_cast<unsigned int>(ToFill.size());

        if (Dim == 0)
            DeAllocate();
        else {
            Allocate(Dim, 0);

            for (unsigned int i = 0; i < Dim; i++)
                Point[i] = ToFill[i];
        }
    }
    explicit MathVector(std::istream &in);
    MathVector(const MathVector &Obj) noexcept;
    MathVector(MathVector &&Obj) noexcept;
    ~MathVector();

    [[nodiscard]] VariableType* MoveIntoPointer() noexcept override;

    void Sterilize(std::ostream& out) const noexcept override;
    static MathVector* FromSterilized(const std::string &obj);
    [[nodiscard]] std::string GetTypeString() const noexcept override;
    std::ostream& operator<<(std::ostream& out) const noexcept override;

    MathVector& operator=(const MathVector &Obj) noexcept;
    MathVector& operator=(MathVector &&Obj) noexcept;
    [[nodiscard]] VariableTypes GetType() const noexcept override {
        return VariableTypes::VT_Vector;
    }

    static MathVector ErrorVector();

    [[nodiscard]] unsigned int Dim() const { return d; }
    [[nodiscard]] bool IsValid() const { return d > 0; }

    double& operator[](unsigned int TargetDim);
    double operator[](unsigned int TargetDim) const;

    [[nodiscard]] double Magnitude() const;
    [[nodiscard]] double Angle() const;

    [[maybe_unused]] static MathVector CrossProduct(const MathVector &One, const MathVector &Two);
    static double DotProduct(const MathVector &One, const MathVector &Two);

    MathVector operator+(const MathVector& in) const;
    MathVector operator-(const MathVector& in) const;
    MathVector operator*(const Scalar& in) const;
    MathVector operator*(double in) const;
    MathVector operator/(const Scalar& in) const;
    MathVector operator/(double in) const;

    bool operator==(const VariableType& in) const noexcept override;
    bool operator!=(const VariableType& in) const noexcept override;

    explicit operator Matrix() const noexcept;

#ifdef _WINDOWS_
    explicit operator POINT() const
    {
        if (Dim() < 2)
            throw std::exception("This object does not have enough dimensions to construct a POINT object.");

        return POINT{ static_cast<LONG>(Point[0]), static_cast<LONG>(Point[1]) };
    }
#endif
};
/*
 * Created by exdisj
 */

#ifndef JASON_MATHVECTOR_H
#define JASON_MATHVECTOR_H

#include "Constraints.h"
#include "../StdCalc.h"
#include "../VariableType.h"
#include "../OperatorException.h"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

class MATH_LIB MathVector : public VariableType {
private:
    void Allocate(unsigned int Dim, double Val);
    void DeAllocate();

    unsigned int d;
    double *Point;

    MathVector();

public:
    [[maybe_unused]] explicit MathVector(unsigned int Dim, double Val = 0.0);
    explicit MathVector(std::istream &in);
    MathVector(const MathVector &Obj) noexcept;
    [[maybe_unused]] MathVector(MathVector &&Obj) noexcept;
    ~MathVector();

    template<std::convertible_to<double>... Args>
    static MathVector FromList(Args... Value) noexcept;

    [[nodiscard]] VariableType* MoveIntoPointer() noexcept override;
    [[nodiscard]] VariableType* Clone() const noexcept override;

    void Sterilize(std::ostream& out) const noexcept override;
    [[maybe_unused]] [[nodiscard]] static MathVector* FromSterilized(const std::string &obj);
    [[nodiscard]] std::string GetTypeString() const noexcept override;
    void Print(std::ostream& out) const noexcept override;

    MathVector& operator=(const MathVector &Obj) noexcept;
    MathVector& operator=(MathVector &&Obj) noexcept;
    [[nodiscard]] VariableTypes GetType() const noexcept override;

    static MathVector ErrorVector();

    [[nodiscard]] unsigned int Dim() const { return d; }
    [[nodiscard]] bool IsValid() const { return d > 0; }

    double& operator[](unsigned int Index);
    double operator[](unsigned int Index) const;

    [[maybe_unused]] [[nodiscard]]double Magnitude() const;
    [[maybe_unused]] [[nodiscard]] double Angle() const;

    [[maybe_unused]] [[nodiscard]] static MathVector CrossProduct(const MathVector &One, const MathVector &Two);
    [[maybe_unused]] [[nodiscard]] static double DotProduct(const MathVector &One, const MathVector &Two);

    MathVector operator+(const MathVector& in) const;
    MathVector operator-(const MathVector& in) const;
    template<typename T> requires IsScalarOrDouble<T>
    MathVector operator*(const T& in) const;
    template<typename T> requires IsScalarOrDouble<T>
    MathVector operator/(const T& in) const;

    MathVector& operator+=(const MathVector& in);
    MathVector& operator-=(const MathVector& in);
    template<typename T> requires IsScalarOrDouble<T>
    MathVector& operator*=(const T& in);
    template<typename T> requires IsScalarOrDouble<T>
    MathVector& operator/=(const T& in);

    bool operator==(const VariableType& in) const noexcept override;
    bool operator!=(const VariableType& in) const noexcept override;
};

#include "MathVectorT.tpp"

#endif //JASON_MATHVECTOR_H
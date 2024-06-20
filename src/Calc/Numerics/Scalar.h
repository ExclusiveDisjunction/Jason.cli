//
// Created by exdisj on 5/26/24.
//

#ifndef JASON_SCALAR_H
#define JASON_SCALAR_H

#include "../VariableType.h"
#include "../OperatorException.h"

class MATH_LIB Scalar;

template<typename T, typename Base, typename NumType>
concept IsBaseOrNumeric = requires
{
    std::is_base_of<T, Base>::value || std::is_same<T, NumType>::value || std::is_nothrow_convertible<T, NumType>::value;
};

template<typename T>
concept IsScalarOrDouble = IsBaseOrNumeric<T, Scalar, double>;

class MATH_LIB Scalar : public VariableType
{
public:
    template<typename T> requires IsScalarOrDouble<T>
    explicit Scalar(const T& Item);
    explicit Scalar(std::istream& in);

    double Data = 0.00;

    [[nodiscard]] VariableTypes GetType() const noexcept override;
    [[nodiscard]] std::string GetTypeString() const noexcept override;
    void Sterilize(std::ostream& out) const noexcept override;

    [[nodiscard]] VariableType* MoveIntoPointer() noexcept override;

    [[nodiscard]] static Scalar* FromSterilize(const std::string& in);
    [[nodiscard]] static Scalar* FromSterilize(std::istream& in);

    template<typename T> requires IsScalarOrDouble<T>
    Scalar operator+(const T& in) const noexcept;
    template<typename T> requires IsScalarOrDouble<T>
    Scalar operator-(const T& in) const noexcept;
    template<typename T> requires IsScalarOrDouble<T>
    Scalar operator*(const T& in) const noexcept;
    template<typename T> requires IsScalarOrDouble<T>
    Scalar operator/(const T& in) const;

    template<typename T> requires IsScalarOrDouble<T>
    Scalar& operator+=(const T& in) noexcept;
    template<typename T> requires IsScalarOrDouble<T>
    Scalar& operator-=(const T& in) noexcept;
    template<typename T> requires IsScalarOrDouble<T>
    Scalar& operator*=(const T& in) noexcept;
    template<typename T> requires IsScalarOrDouble<T>
    Scalar& operator/=(const T& in);

    [[nodiscard]] long long ToLongNoRound() const;
    [[nodiscard]] long long ToLongTrunc() const noexcept;

    template<typename T> requires IsScalarOrDouble<T>
    [[nodiscard]] Scalar Pow(const T& in) const noexcept;

    bool operator==(const VariableType& obj) const noexcept override;
    bool operator!=(const VariableType& obj) const noexcept override;

    std::ostream& operator<<(std::ostream& out) const noexcept override;

    constexpr explicit operator double() const noexcept;
};

#include "Scalar.cpp"

#endif //JASON_SCALAR_H

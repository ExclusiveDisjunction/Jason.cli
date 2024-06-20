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

class Scalar : public VariableType
{
public:
    template<typename T> requires IsScalarOrDouble<double>
    explicit Scalar(const T& Item);

    [[nodiscard]] VariableTypes GetType() const noexcept override;
    [[nodiscard]] std::string GetTypeString() const noexcept override;
    void Sterilize(std::ostream& out) const noexcept override;

    [[nodiscard]] static Scalar* FromSterilize(const std::string& in);
    [[nodiscard]] static Scalar* FromSterilize(std::istream& in);

    template<typename T> requires std::convertible_to<T, double>
    Scalar operator+(const T& in) const noexcept;
    template<typename T> requires std::convertible_to<T, double>
    Scalar operator-(const T& in) const noexcept;
    template<typename T> requires std::convertible_to<T, double>
    Scalar operator*(const T& in) const noexcept;
    template<typename T> requires std::convertible_to<T, double>
    Scalar operator/(const T& in) const noexcept;

    template<typename T> requires std::convertible_to<T, double>
    Scalar& operator+=(const T& in) noexcept;
    template<typename T> requires std::convertible_to<T, double>
    Scalar& operator-=(const T& in) noexcept;
    template<typename T> requires std::convertible_to<T, double>
    Scalar& operator*=(const T& in) noexcept;
    template<typename T> requires std::convertible_to<T, double>
    Scalar& operator/=(const T& in);

    long long ToLongNoRound() const;
    long long ToLongTrunc() const noexcept;

    template<typename T> requires IsScalarOrDouble<T>
    [[nodiscard]] Scalar Pow(const T& in) const noexcept;

    bool operator==(const VariableType& obj) const noexcept override;
    bool operator!=(const VariableType& obj) const noexcept override;

    std::ostream& operator<<(std::ostream& out) const noexcept override;

    virtual constexpr explicit operator double() const noexcept = 0; 
};

class Integer : public Scalar
{
protected:
    [[nodiscard]] std::string GetScaType() const noexcept override;
public:
    explicit Integer(long long i);
    explicit Integer(std::istream& in);

    [[nodiscard]] VariableType* MoveIntoPointer() noexcept override;

    long long Data = 0;

    template<typename T> requires IsBaseOrNumeric<T, Integer, long long>
    Integer operator+(const T& in) const noexcept;
    template<typename T> requires IsBaseOrNumeric<T, Integer, long long>
    Integer operator-(const T& in) const noexcept;
    template<typename T> requires IsBaseOrNumeric<T, Integer, long long>
    Integer operator*(const T& in) const noexcept;
    template<typename T> requires IsBaseOrNumeric<T, Integer, long long>
    Integer operator/(const T& in) const noexcept;
    template<typename T> requires IsBaseOrNumeric<T, Integer, long long>
    Integer operator%(const T& in) const noexcept;

    template<typename T> requires IsBaseOrNumeric<T, Integer, long long>
    Integer operator+=(const T& in) const noexcept;
    template<typename T> requires IsBaseOrNumeric<T, Integer, long long>
    Integer operator-=(const T& in) const noexcept;
    template<typename T> requires IsBaseOrNumeric<T, Integer, long long>
    Integer operator*=(const T& in) const noexcept;
    template<typename T> requires IsBaseOrNumeric<T, Integer, long long>
    Integer operator/=(const T& in) const noexcept;
    template<typename T> requires IsBaseOrNumeric<T, Integer, long long>
    Integer operator%=(const T& in) const noexcept;

    constexpr explicit operator double() const noexcept override;
    constexpr explicit operator long long() const noexcept;
};

class Fraction : public Scalar
{
private:
    [[nodiscard]] std::string GetScaType() const noexcept override;

    long long A = 0;
    long long B = 0;
public:
    Fraction(long long Num, long long Denom);
    explicit Fraction(std::istream& in);

    [[nodiscard]] VariableType* MoveIntoPointer() noexcept override;

    std::pair<long long, long long> GetVals() const noexcept;
    void SetVals(const std::pair<long long, long long>& New);
    void SetVals(long long Num, long long Denom);

    Fraction operator+(const Fraction& in) const noexcept;
    Fraction operator-(const Fraction& in) const noexcept;
    Fraction operator*(const Fraction& in) const noexcept;
    Fraction operator/(const Fraction& in) const noexcept;

    Fraction& operator+=(const Fraction& in) const noexcept;
    Fraction& operator-=(const Fraction& in) const noexcept;
    Fraction& operator*=(const Fraction& in) const noexcept;
    Fraction& operator/=(const Fraction& in) const noexcept;

    constexpr explicit operator double() const noexcept override;
};

class RealNumber : public Scalar
{
private:
    [[nodiscard]] std::string GetScaType() const noexcept override;
public:
    explicit RealNumber(double val);
    explicit RealNumber(std::istream& in);

    [[nodiscard]] VariableType* MoveIntoPointer() noexcept override;

    double Data;

    template<typename T> requires IsScalarOrDouble<T>
    RealNumber& operator+=(const T& in) const noexcept;
    template<typename T> requires IsScalarOrDouble<T>
    RealNumber& operator-=(const T& in) const noexcept;
    template<typename T> requires IsScalarOrDouble<T>
    RealNumber& operator*=(const T& in) const noexcept;
    template<typename T> requires IsScalarOrDouble<T>
    RealNumber& operator/=(const T& in) const noexcept;

    //Since Scalar has all operators that are valid for Real numbers, this class has to do nothing.
    constexpr explicit operator double() const noexcept override;
};


#endif //JASON_SCALAR_H

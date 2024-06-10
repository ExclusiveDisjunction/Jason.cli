//
// Created by exdisj on 5/26/24.
//

#ifndef JASON_SCALAR_H
#define JASON_SCALAR_H

#include "../VariableType.h"
#include "../OperatorException.h"

class MATH_LIB Complex;
class MATH_LIB Imaginary;

class MATH_LIB Scalar;
class MATH_LIB Integer;
class MATH_LIB Fraction;
class MATH_LIB RealNumber;

class Scalar : public VariableType
{
protected:
    //Used to get the type of scalar from the derived types.
    [[nodiscard]] virtual std::string GetScaType() const noexcept = 0;
public:
    [[nodiscard]] VariableTypes GetType() const noexcept override;
    [[nodiscard]] std::string GetTypeString() const noexcept override;
    void Sterilize(std::ostream& out) const noexcept override;

    [[nodiscard]] static Scalar* FromSterilize(const std::string& in);
    [[nodiscard]] static Scalar* FromSterilize(std::istream& in);

    template<typename T> requires std::convertible_to<T, double>
    RealNumber operator+(const T& in) const noexcept;
    template<typename T> requires std::convertible_to<T, double>
    RealNumber operator-(const T& in) const noexcept;
    template<typename T> requires std::convertible_to<T, double>
    RealNumber operator*(const T& in) const noexcept;
    template<typename T> requires std::convertible_to<T, double>
    RealNumber operator/(const T& in) const noexcept;

    RealNumber operator+(double in) const noexcept;
    RealNumber operator-(double in) const noexcept;
    RealNumber operator*(double in) const noexcept;
    RealNumber operator/(double in) const noexcept;

    Complex operator+(const Imaginary& in) const noexcept;
    Complex operator-(const Imaginary& in) const noexcept;

    Scalar& operator+=(const Scalar& in) const noexcept;
    Scalar& operator-=(const Scalar& in) const noexcept;
    Scalar& operator*=(const Scalar& in) const noexcept;
    Scalar& operator/=(const Scalar& in) const noexcept;

    virtual Scalar& operator+=(double in) const noexcept = 0;
    virtual Scalar& operator-=(double in) const noexcept = 0;
    virtual Scalar& operator*=(double in) const noexcept = 0;
    virtual Scalar& operator/=(double in) const noexcept = 0;

    template<typename T> requires std::convertible_to<T, double>
    [[nodiscard]] RealNumber Pow(double in) const noexcept;

    bool operator==(const VariableType& obj) const noexcept override;
    bool operator!=(const VariableType& obj) const noexcept override;

    std::ostream& operator<<(std::ostream& out) const noexcept override;

    virtual constexpr explicit operator double() const noexcept = 0;
    constexpr explicit operator Complex() const noexcept;
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

    template<typename T> requires std::convertible_to<T, long long>
    Integer operator+(const T& in) const noexcept;
    template<typename T> requires std::convertible_to<T, long long>
    Integer operator-(const T& in) const noexcept;
    template<typename T> requires std::convertible_to<T, long long>
    Integer operator*(const T& in) const noexcept;
    template<typename T> requires std::convertible_to<T, long long>
    Integer operator/(const T& in) const noexcept;
    template<typename T> requires std::convertible_to<T, long long>
    Integer operator%(const T& in) const noexcept;

    template<typename T> requires std::convertible_to<T, long long>
    Integer& operator+=(const T& in) const noexcept;
    template<typename T> requires std::convertible_to<T, long long>
    Integer& operator-=(const T& in) const noexcept;
    template<typename T> requires std::convertible_to<T, long long>
    Integer& operator*=(const T& in) const noexcept;
    template<typename T> requires std::convertible_to<T, long long>
    Integer& operator/=(const T& in) const noexcept;
    template<typename T> requires std::convertible_to<T, long long>
    Integer& operator%=(const T& in) const noexcept;

    Scalar& operator+=(double in) const noexcept override;
    Scalar& operator-=(double in) const noexcept override;
    Scalar& operator*=(double in) const noexcept override;
    Scalar& operator/=(double in) const noexcept override;

    template<typename T> requires std::convertible_to<T, long long>
    [[nodiscard]] Integer Pow(const T& in) const noexcept;

    constexpr explicit operator double() const noexcept override;
    constexpr explicit operator long long() const noexcept;
};
class Fraction : public Scalar
{
private:
    [[nodiscard]] std::string GetScaType() const noexcept override;
public:
    Fraction(long long Num, long long Denom);
    explicit Fraction(std::istream& in);

    [[nodiscard]] VariableType* MoveIntoPointer() noexcept override;

    long long A = 0;
    long long B = 0;

    Fraction operator+(const Fraction& in) const noexcept;
    Fraction& operator+=(const Fraction& in) const noexcept;
    Scalar& operator+=(double in) const noexcept override;
    Fraction operator-(const Fraction& in) const noexcept;
    Fraction& operator-=(const Fraction& in) const noexcept;
    Scalar& operator-=(double in) const noexcept override;
    Fraction operator*(const Fraction& in) const noexcept;
    Fraction& operator*=(const Fraction& in) const noexcept;
    Scalar& operator*=(double in) const noexcept override;
    Fraction operator/(const Fraction& in) const noexcept;
    Fraction& operator/=(const Fraction& in) const noexcept;
    Scalar& operator/=(double in) const noexcept override;

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

    Scalar& operator+=(double in) const noexcept override;
    Scalar& operator-=(double in) const noexcept override;
    Scalar& operator*=(double in) const noexcept override;
    Scalar& operator/=(double in) const noexcept override;

    //Since Scalar has all operators that are valid for Real numbers, this class has to do nothing.
    constexpr explicit operator double() const noexcept override;
};


#endif //JASON_SCALAR_H

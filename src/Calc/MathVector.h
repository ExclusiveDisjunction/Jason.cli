#pragma once

#include "StdCalc.h"
#include "VariableType.h"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

class MATH_LIB Matrix;

class MATH_LIB MathVector : public VariableType {
private:
    void Allocate(unsigned int Dim, double Val);

    void DeAllocate();

    unsigned int _Dim = 1;
    double *Point = nullptr;

public:
    explicit MathVector(double Val) : MathVector(1, Val) {}
    explicit MathVector(unsigned int Dim, double Val = 0.0);
    MathVector(unsigned int Dim, double *Point);
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
    explicit MathVector(std::istream &in) {
        std::string header;
        in >> header;
        if (header != "VEC")
            throw std::logic_error("Cannot construct vector from stream because the header is not the stream.");

        in >> this->_Dim;
        if (_Dim < 0)
            throw std::logic_error("Dimension is negative.");
        else if (_Dim == 0) {
            DeAllocate();
            return;
        } else //Dim is positive
        {
            Allocate(_Dim, 0);
            for (int i = 0; i < _Dim; i++) {
                if (!in)
                    throw std::logic_error("There is not enough inputs to match the dimensions.");

                in >> this->Point[i];
            }
        }
    }
    MathVector(const MathVector &Obj) noexcept;
    MathVector(MathVector &&Obj) noexcept;
    ~MathVector();

    void Sterilize(std::ostream& out) const noexcept override;
    static MathVector *FromSterilized(const std::string &obj) {
        try {
            std::stringstream ss(obj);
            return new MathVector(ss);
        }
        catch (std::logic_error &e) {
            throw e;
        }
    }
    [[nodiscard]] std::string GetTypeString() const noexcept override
    {
        std::stringstream ss;
        ss << "(Vector:" << this->_Dim << ")";
        return ss.str();
    }
    std::ostream& operator<<(std::ostream& out) const noexcept override;

    MathVector &operator=(const MathVector &Obj) noexcept;
    MathVector &operator=(MathVector &&Obj) noexcept;
    [[nodiscard]] VariableTypes GetType() const noexcept override {
        return VariableTypes::VT_Vector;
    }

    static MathVector ErrorVector();

    [[nodiscard]] unsigned int Dim() const { return _Dim; }
    [[nodiscard]] bool IsValid() const { return _Dim > 0; }

    double &operator[](unsigned int TargetDim) const;

    [[nodiscard]] double Magnitude() const;
    [[nodiscard]] double Angle() const;

    static MathVector CrossProduct(const MathVector &One, const MathVector &Two);
    static double DotProduct(const MathVector &One, const MathVector &Two);

    VariableType* operator+(const VariableType& in) const noexcept override;
    VariableType* operator-(const VariableType& in) const noexcept override;
    VariableType* operator*(const VariableType& in) const noexcept override;
    VariableType* operator/(const VariableType& in) const noexcept override;

    bool operator==(const VariableType& in) const noexcept override;
    bool operator!=(const VariableType& in) const noexcept override;

    explicit operator Matrix() const;

#ifdef _WINDOWS_
    explicit operator POINT() const
    {
        if (Dim() < 2)
            throw std::exception("This object does not have enough dimensions to construct a POINT object.");

        return POINT{ static_cast<LONG>(Point[0]), static_cast<LONG>(Point[1]) };
    }
#endif
};
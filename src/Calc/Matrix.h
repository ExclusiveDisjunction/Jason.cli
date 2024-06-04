#pragma once

#include "StdCalc.h"
#include "VariableType.h"

class MATH_LIB Scalar;
class MATH_LIB MathVector;

/// <summary>
/// Represents a rectangular arrangement of numbers for calculations, given some row and column definition.
/// </summary>
class MATH_LIB Matrix : public VariableType
{
private:
    /// <summary>
    /// The data stored in the matrix.
    /// </summary>
    double** Data = nullptr;
    unsigned int m = 0;
    unsigned int n = 0;

    void Allocate(unsigned int NewRows, unsigned int NewColumns, double Value = 0);
    void DeAllocate();

public:
    Matrix(unsigned int Rows, unsigned int Columns, double Value = 0) noexcept;
    Matrix(std::istream& in);
    Matrix(const Matrix& Other) noexcept;
    Matrix(Matrix&& Other) noexcept;
    ~Matrix();

    [[nodiscard]] unsigned int Rows() const { return m; }
    [[nodiscard]] unsigned int Columns() const { return n; }
    [[nodiscard]] bool IsValid() const { return m != 0 && n != 0; }

    [[nodiscard]] VariableType* MoveIntoPointer() noexcept override;

    [[nodiscard]] VariableTypes GetType() const noexcept override
    {
        return VariableTypes::VT_Matrix;
    }
    void Sterilize(std::ostream& out) const noexcept override;
    static Matrix* FromSterilize(const std::string& sterilized)
    {
        try
        {
            std::stringstream ss(sterilized);
            return new Matrix(ss);
        }
        catch (std::logic_error& e)
        {
            throw e;
        }
    }
    [[nodiscard]] std::string GetTypeString() const noexcept override
    {
        std::stringstream ss;
        ss << "(Matrix:" << m << "x" << n << ")";
        return ss.str();
    }

    static Matrix ErrorMatrix();
    static Matrix Identity(unsigned int Size);
    static Matrix Identity(unsigned int Rows, unsigned int Cols);
    static Matrix RandomMatrix(unsigned int Rows, unsigned int Columns, bool Integers);

    const double* operator[](unsigned int Row) const;
    double* operator[](unsigned int Row);

    Matrix& operator=(const Matrix& Other) noexcept;
    Matrix& operator=(Matrix&& Other) noexcept;

    [[nodiscard]] Matrix Extract(unsigned int StartI, unsigned int StartJ, unsigned int RowCount, unsigned int ColumnCount);

    void RowSwap(unsigned int OrigRow, unsigned int NewRow);
    void RowAdd(unsigned int OrigRow, double Fac, unsigned int TargetRow);

    [[nodiscard]] double Determinant() const;

    [[nodiscard]] Matrix Invert() const;
    [[nodiscard]] Matrix Transpose() const;

    void REF();
    void RREF();

    Matrix operator|(const Matrix& Two) const;
    Matrix operator+(const Matrix& Two) const;
    Matrix operator-(const Matrix& Two) const;
    Matrix operator*(const Matrix& Two) const;
    Matrix operator*(const Scalar& Two) const;
    Matrix operator*(double Two) const;
    Matrix operator/(const Scalar& Two) const;
    Matrix operator/(double Two) const;
    Matrix Pow(const Scalar& Two) const;
    Matrix Pow(double Two);

    bool operator==(const VariableType& two) const noexcept override;
    bool operator!=(const VariableType& two) const noexcept override;

    std::ostream& operator<<(std::ostream& out) const noexcept override;
};
#pragma once

#include "StdCalc.h"
#include "VariableType.h"
#include <vector>

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

    void Allocate(unsigned int NewRows, unsigned int NewColumns, double Value = 0) noexcept;
    void DeAllocate() noexcept;

    [[nodiscard]] std::vector<std::pair<bool, unsigned long>> GetColumnWidthSchematic() const;
    std::string GetRowString(unsigned row, std::vector<std::pair<bool, unsigned long>>& schema, char open, char close) const;

    Matrix();

public:
    Matrix(unsigned int Rows, unsigned int Columns, double Value = 0) noexcept;
    explicit Matrix(std::istream& in);
    Matrix(const Matrix& Other) noexcept;
    Matrix(Matrix&& Other) noexcept;
    ~Matrix();

    [[nodiscard]] unsigned int Rows() const { return m; }
    [[nodiscard]] unsigned int Columns() const { return n; }
    [[nodiscard]] bool IsValid() const { return m != 0 && n != 0 && this->Data; }

    [[nodiscard]] VariableType* MoveIntoPointer() noexcept override;

    [[nodiscard]] VariableTypes GetType() const noexcept override;
    void Sterilize(std::ostream& out) const noexcept override;
    std::ostream& operator<<(std::ostream& out) const noexcept override;

    [[maybe_unused]] [[maybe_unused]] [[nodiscard]] [[maybe_unused]] static Matrix* FromSterilize(const std::string& sterilized);
    [[nodiscard]] std::string GetTypeString() const noexcept override;

    [[nodiscard]] static Matrix ErrorMatrix();
    [[nodiscard]] static Matrix Identity(unsigned int Size);
    [[nodiscard]] static Matrix Identity(unsigned int Rows, unsigned int Cols);
    [[nodiscard]] [[maybe_unused]] static Matrix RandomMatrix(unsigned int Rows, unsigned int Columns, bool Integers);

    const double* operator[](unsigned int Row) const;
    double* operator[](unsigned int Row);

    Matrix& operator=(const Matrix& Other) noexcept;
    Matrix& operator=(Matrix&& Other) noexcept;

    [[nodiscard]] Matrix Extract(unsigned int StartI, unsigned int StartJ, unsigned int RowCount, unsigned int ColumnCount);

    void RowSwap(unsigned int OrigRow, unsigned int NewRow);
    void RowAdd(unsigned int OrigRow, double Fac, unsigned int TargetRow);

    [[maybe_unused]] [[nodiscard]] double Determinant() const;

    [[maybe_unused]] [[nodiscard]] Matrix Invert() const;
    [[maybe_unused]] [[nodiscard]] Matrix Transpose() const;

    void RowEchelonForm();
    void ReducedRowEchelonForm();

    Matrix operator|(const Matrix& Two) const;
    Matrix operator+(const Matrix& Two) const;
    Matrix operator-(const Matrix& Two) const;
    Matrix operator*(const Matrix& Two) const;
    Matrix operator*(const Scalar& Two) const;
    Matrix operator*(double Two) const;
    Matrix operator/(const Scalar& Two) const;
    Matrix operator/(double Two) const;
    [[nodiscard]] Matrix Pow(const Scalar& Two) const;
    [[nodiscard]] Matrix Pow(double Two) const;

    bool operator==(const VariableType& two) const noexcept override;
    bool operator!=(const VariableType& two) const noexcept override;
};
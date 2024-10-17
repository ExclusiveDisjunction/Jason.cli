/*
 * Created by exdisj
 */

#ifndef JASON_MATRIX_H
#define JASON_MATRIX_H

#include "Constraints.h"
#include "../StdCalc.h"
#include "../VariableType.h"
#include "../OperatorException.h"
#include <vector>

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
    double** Data;
    unsigned int m;
    unsigned int n;

    void Allocate(unsigned int NewRows, unsigned int NewColumns, double Value = 0) noexcept;
    void DeAllocate() noexcept;

    [[nodiscard]] std::vector<std::pair<bool, unsigned long>> GetColumnWidthSchematic() const;
    [[nodiscard]] bool GetRowString(std::ostream& out, unsigned row, std::vector<std::pair<bool, unsigned long>>& schema, char open, char close) const;

    Matrix();

public:
    Matrix(unsigned int Rows, unsigned int Columns) noexcept;
    template<std::convertible_to<double>... args>
    [[maybe_unused]] Matrix(unsigned Rows, unsigned Columns, args... vals);
    [[maybe_unused]] explicit Matrix(const MathVector& in);
    explicit Matrix(std::istream& in);
    Matrix(const Matrix& Other) noexcept;
    Matrix(Matrix&& Other) noexcept;
    ~Matrix();

    friend void PrintMatrixSingleLine(std::ostream&, const Matrix&) noexcept;

    [[nodiscard]] unsigned int Rows() const { return m; }
    [[nodiscard]] unsigned int Columns() const { return n; }
    [[nodiscard]] bool IsValid() const { return m != 0 && n != 0 && this->Data; }

    [[nodiscard]] VariableType* MoveIntoPointer() noexcept override;
    [[nodiscard]] std::unique_ptr<VariableType> Clone() const noexcept override;

    [[nodiscard]] VariableTypes GetType() const noexcept override;
    void Sterilize(std::ostream& out) const noexcept override;
    void Print(std::ostream& out) const noexcept override;

    [[nodiscard]] [[maybe_unused]] static Matrix* FromSterilize(const std::string& sterilized);
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

    template<typename T> requires IsScalarOrDouble<T>
    Matrix operator*(const T& Two) const;
    template<typename T> requires IsScalarOrDouble<T>
    Matrix operator/(const T& Two) const;

    Matrix& operator+=(const Matrix& Two);
    Matrix& operator-=(const Matrix& Two);
    Matrix& operator*=(const Matrix& Two);
    template<typename T> requires IsScalarOrDouble<T>
    Matrix& operator*=(const T& Two);
    template<typename T> requires IsScalarOrDouble<T>
    Matrix& operator/=(const T& Two);

    [[nodiscard]] Matrix Pow(unsigned long long Two) const;

    bool operator==(const VariableType& two) const noexcept override;
    bool operator!=(const VariableType& two) const noexcept override;
};

struct MatrixSingleLinePrint {
    const Matrix& Target;
};
inline MatrixSingleLinePrint PrintMatrixOneLine(const Matrix& obj) noexcept
{
    return MatrixSingleLinePrint { obj };
}

std::ostream& operator<<(std::ostream& out, const MatrixSingleLinePrint& Obj);

#include "MatrixT.tpp"

#endif //JASON_MATRIX_H
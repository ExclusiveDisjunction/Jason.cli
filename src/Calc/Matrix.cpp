#include "Matrix.h"

#include "MathVector.h"
#include "Scalar.h"

#include <random>
#include <utility>

Matrix::Matrix()
{
    DeAllocate();
}
Matrix::Matrix(unsigned int Rows, unsigned int Columns, double Value) noexcept
{
    Allocate(Rows, Columns, Value);
}
Matrix::Matrix(std::istream& in)
{

}
Matrix::Matrix(const Matrix& Other) noexcept
{
    Allocate(Other.Rows(), Other.Columns());

    for (unsigned int i = 0; i < m; i++)
        for (unsigned int j = 0; j < n; j++)
            Data[i][j] = Other.Data[i][j];
}
Matrix::Matrix(Matrix&& Other) noexcept
{
    this->Data = std::exchange(Other.Data, nullptr);
    this->m = std::exchange(Other.m, 0);
    this->n = std::exchange(Other.n, 0);

    Other.DeAllocate();
}
Matrix::~Matrix()
{
    DeAllocate();
}

Matrix& Matrix::operator=(const Matrix& Other) noexcept
{
    if (Data == Other.Data) //Self assignment
        return *this;

    if (Other.Rows() != m || Other.Columns() != n)
        Allocate(Other.Rows(), Other.Columns());

    for (unsigned int i = 0; i < m; i++)
        for (unsigned int j = 0; j < n; j++)
            Data[i][j] = Other.Data[i][j];

    return *this;
}
Matrix& Matrix::operator=(Matrix&& Other) noexcept
{
    this->DeAllocate();

    this->Data = std::exchange(Other.Data, nullptr);
    this->m = std::exchange(Other.m, 0);
    this->n = std::exchange(Other.n, 0);

    Other.DeAllocate();

    return *this;
}

void Matrix::Allocate(unsigned int NewRows, unsigned int NewColumns, double Value) noexcept
{
    if (NewRows != m || NewColumns != n)
    {
        DeAllocate();

        m = NewRows;
        n = NewColumns;

        Data = new double* [NewRows];
        for (unsigned int i = 0; i < NewRows; i++)
            Data[i] = new double[NewColumns] { Value };
    }
}
void Matrix::DeAllocate() noexcept
{
    if (Data)
    {
        for (unsigned int i = 0; i < m; i++)
        {
            delete[] Data[i];
            Data[i] = nullptr;
        }
        delete[] Data;

        Data = nullptr;
    }
    m = 0;
    n = 0;
}

Matrix Matrix::ErrorMatrix()
{
    return Matrix();
}
Matrix Matrix::Identity(unsigned int Size)
{
    return Identity(Size, Size);
}
Matrix Matrix::Identity(unsigned int Rows, unsigned int Cols)
{
    Matrix result(Rows, Cols, 0);
    for (unsigned i = 0; i < Rows && i < Cols; i++)
        result[i][i] = 1;

    return result;
}
Matrix Matrix::RandomMatrix(unsigned int Rows, unsigned int Columns, bool Integers)
{
    Matrix Return(Rows, Columns);
    if (!Return.Data)
        throw std::logic_error("Cannot construct a matrix of zero size.");

    std::random_device dev;
    std::mt19937 engine(dev());

    if (Integers)
    {
        std::uniform_int_distribution dist(-10, 10);

        for (unsigned int i = 0; i < Rows; i++)
            for (unsigned int j = 0; j < Columns; j++)
                Return.Data[i][j] = dist(engine);
    }
    else
    {
        std::uniform_real_distribution dist(-10.0, 10.0);

        for (unsigned int i = 0; i < Rows; i++)
            for (unsigned int j = 0; j < Columns; j++)
                Return.Data[i][j] = dist(engine);
    }

    return Return;
}

VariableType* Matrix::MoveIntoPointer() noexcept
{
    Matrix* Return = new Matrix();
    Return.Data = std::exchange(this->Data, nullptr);
    Return.m = std::exchange(this->m, nullptr);
    Return.n = std::exchange(this->n, nullptr);

    this->DeAllocate();

    return Return;
}

const double* Matrix::operator[](unsigned int Row) const
{
    if (Row > m)
        throw std::logic_error("Out of bounds");

    return Data[Row];
}
double* Matrix::operator[](unsigned int Row)
{
    if (Row > m)
        throw std::logic_error("Out of bounds");

    return Data[Row];
}

VariableTypes Matrix::GetType() const noexcept
{
    return VariableTypes::VT_Matrix;
}
void Matrix::Sterilize(std::ostream& out) const noexcept
{
    out << "MAT " << this->m << ' ' << this->n << ' ';
    for (unsigned i = 0; i < this->m && this->Data; i++)
        for (unsigned j = 0; j < this->n; j++)
            out << this->Data[i][j] << ' ';
}
Matrix* Matrix::FromSterilize(const std::string& sterilized)
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
std::string Matrix::GetTypeString() const noexcept
{
    std::stringstream ss;
    ss << "(Matrix:" << m << "x" << n << ")";
    return ss.str();
}

Matrix Matrix::Extract(unsigned int StartI, unsigned int StartJ, unsigned int RowCount, unsigned int ColumnCount)
{
    /*
     * I want to split this in two parts. This function will return a MatrixExtrusion, and then the other part will
     * return a MatrixMinor.
     */

    if (RowCount == 0 || ColumnCount == 0)
        return Matrix::ErrorMatrix();

    if ((StartI + RowCount - 1) > m || (StartJ + ColumnCount - 1) > n)
        throw std::logic_error("The index was out of range for that size.");

    Matrix Return(RowCount, ColumnCount);
    if (!Return.Data)
        return Matrix::ErrorMatrix();

    for (unsigned int i = StartI, ip = 0; i < StartI + RowCount - 1; i++, ip++)
        for (unsigned int j = StartJ, jp = 0; j < StartJ + ColumnCount - 1; j++, jp++)
            Return.Data[ip][jp] = Data[i][j];

    return Return;
}

void Matrix::RowSwap(unsigned int OrigRow, unsigned int NewRow)
{
    if (OrigRow == NewRow || OrigRow > m || NewRow > m)
        return;

    std::swap(Data[OrigRow], Data[NewRow]);
}
void Matrix::RowAdd(unsigned int OrigRow, double Fac, unsigned int TargetRow)
{
    if (Fac == 0)
        throw std::logic_error("The factor of multiplication cannot be zero.");

    for (unsigned int j = 0; j < n; j++)
        Data[TargetRow][j] += Data[OrigRow][j] * Fac;
}

double Matrix::Determinant() const
{
    if (Rows() != Columns())
        throw std::logic_error("The matrix must be square");

    /*
     * To do the determinant, we will have to get the minors of the matrix, and therefore requires the MatrixMinor.
     */
    return 0;
}

Matrix Matrix::Invert() const
{
    if (Rows() != Columns())
        throw std::logic_error("The matrix must be square");

    Matrix Id = Identity(m);
    Matrix Aug = Id | *this;
    Aug.RREF();

    Matrix Left = Aug.Extract(0, 0, m, m), Right = Aug.Extract(0, 4, m, m);
    if (Right != Id)
        return ErrorMatrix();
    else
        return Left;
}
Matrix Matrix::Transpose() const
{
    Matrix Return(m, n);
    if (!Return.Data)
        throw std::logic_error("Cannot transpose an empty matrix.");

    for (unsigned int i = 0; i < m; i++)
        for (unsigned int j = 0; j < n; j++)
            Return.Data[j][i] = Data[i][j];

    return Return;
}

void Matrix::REF()
{
    unsigned int nr = Rows(), nc = Columns();

    for (unsigned int r = 0; r < nr; r++)
    {
        bool AllZeroes = true;
        for (unsigned int c = 0; c < nc; c++)
        {
            if (Data[r][c] != 0)
            {
                AllZeroes = false;
                break;
            }
        }

        if (AllZeroes == true)
            RowSwap(r, nr);
    }

    unsigned int p = 0;
    while (p < nr && p < nc)
    {
    NextPivot:
        unsigned int r = 1;
        while (Data[p][p] == 0)
        {
            if (p + r <= nr)
            {
                p++;
                goto NextPivot;
            }

            RowSwap(p, p + r);
            r++;
        }
        for (r = 1; r < nr - p; r++)
        {
            if (Data[(p + r - 1) * Columns() + p] != 0)
            {
                double x = -Data[p + r][p] / Data[p][p];
                for (unsigned int c = p; c < nc; c++)
                    Data[p + r][c] = Data[p][c] * x + Data[p + r][c];
            }
        }
        p++;
    }
}
void Matrix::RREF()
{
    unsigned int Lead = 0;
    unsigned int Rows = this->Rows(), Columns = this->Columns();
    for (unsigned int r = 0; r < Rows; r++)
    {
        if (Columns < Lead)
            return;
        unsigned i = r;
        while (Data[i][Lead] == 0)
        {
            i++;
            Lead++;
            if (Columns == Lead)
                return;
        }

        if (i != r)
            RowSwap(i, r);

        for (unsigned int j = 0; j < Columns; j++)
            Data[r][j] /= Data[r][Lead];

        for (unsigned int j = 0; j < Rows; j++)
        {
            if (j != r)
                RowAdd(r, -Data[j][Lead], j);
        }
        Lead++;
    }
}

bool Matrix::operator==(const VariableType& two) const noexcept
{
    try
    {
        const auto& a = *this;
        const auto& b = dynamic_cast<const Matrix&>(two);

        if (!a.IsValid() && b.IsValid()) //If both are empty then they are equal
            return true;

        if (!a.IsValid() || !b.IsValid() || a.m != b.m || a.n != b.n)
            return false;

        for (unsigned i = 0; i < a.m; i++)
            for (unsigned j = 0; j < a.n; j++)
                if (a.Data[i][j] != b.Data[i][j])
                    return false;

        return true;
    }
    catch (std::bad_cast& e)
    {
        return false;
    }
}
bool Matrix::operator!=(const VariableType& two) const noexcept
{
    try
    {
        const auto& a = *this;
        const auto& b = dynamic_cast<const Matrix&>(two);

        if (!a.IsValid() && b.IsValid()) //If both are empty then they are equal
            return false;

        if (!a.IsValid() || !b.IsValid() || a.m != b.m || a.n != b.n)
            return true;

        for (unsigned i = 0; i < a.m; i++)
            for (unsigned j = 0; j < a.n; j++)
                if (a.Data[i][j] != b.Data[i][j])
                    return true;

        return false;
    }
    catch (std::bad_cast& e)
    {
        return true;
    }
}

std::ostream& Matrix::operator<<(std::ostream& out) const noexcept
{
    if (!this->IsValid())
    {
        out << "[ ]";
        return out;
    }

    auto schema = this->GetColumnWidthSchematic();

    if (this->m == 1)
        out << this->GetRowString(0, schema, '[', ']');
    else
    {
        // ⌊ ⌋ ⌈ ⌉ |

        for (unsigned i = 0; i < m; i++)
        {
            char open, close;
            if (i == 0 || i == m - 1) //First and last rows
            {
                open = '[';
                close = ']';
            }
            else
                open = close = '|';

            out << this->GetRowString(i, schema, open, close) << '\n';
        }
    }

    return out;
}

Matrix Matrix::operator|(const Matrix& Two) const
{
    if (!IsValid() || !Two.IsValid())
        throw OperatorException('|', GetTypeString(), Two.GetTypeString(), "Cannot augment empty matrix");

    if (n != Two.n)
        throw OperatorException('|', GetTypeString(), Two.GetTypeString(), "Row dimensions do not match.");

    unsigned int OneRows = m, OneColumns = n, TwoColumns = Two.n;
    Matrix Return(OneRows, OneColumns + TwoColumns);

    if (!Return.IsValid())
        throw std::exception(); //Not supposed to happen, just in case though.

    for (unsigned int i = 0; i < OneRows; i++)
    {
        for (unsigned int j = 0; j < OneColumns; j++)
            Return.Data[i][j] = Two.Data[i][j];
        for (unsigned int j = OneColumns, jp = 0; j < OneColumns + TwoColumns; j++, jp++)
            Return.Data[i][j] = Two.Data[i][jp];
    }

    return Return;
}
Matrix Matrix::operator+(const Matrix& Two) const
{
    if (!this->IsValid() || !Two.IsValid())
        throw OperatorException('+', this->GetTypeString(), Two.GetTypeString(), "Empty Matrix");

    if (this->m != Two.m || this->n != Two.n)
        throw OperatorException('+', this->GetTypeString(), Two.GetTypeString(), "Dimension Mismatch");

    Matrix result(*this);
    for (unsigned i = 0; i < result.m && result.Data; i++)
        for (unsigned j = 0; j < result.n; j++)
            result.Data[i][j] += Two.Data[i][j];

    return result;
}
Matrix Matrix::operator-(const Matrix& Two) const
{
    if (!this->IsValid() || !Two.IsValid())
        throw OperatorException('-', this->GetTypeString(), Two.GetTypeString(), "Empty Matrix");

    if (this->m != Two.m || this->n != Two.n)
        throw OperatorException('-', this->GetTypeString(), Two.GetTypeString(), "Dimension Mismatch");

    Matrix result(*this);
    for (unsigned i = 0; i < result.m && result.Data; i++)
        for (unsigned j = 0; j < result.n; j++)
            result.Data[i][j] -= Two.Data[i][j];

    return result;
}
Matrix Matrix::operator*(const Matrix& Two) const
{
    if (!this->IsValid() || !Two.IsValid())
        throw OperatorException('*', this->GetTypeString(), Two.GetTypeString(), "Empty Matrix");

    if (this->n != Two.m)
        throw OperatorException('*', this->GetTypeString(), Two.GetTypeString(), "Dimension mismatch");

    unsigned r = this->m, c = Two.n;
    Matrix result(r, c);

    for (unsigned i = 0; i < r && result.Data; i++)
    {
        for (unsigned j = 0; j < c; j++)
        {
            result.Data[i][j] = 0.00;
            for (unsigned k = 0; k < Two.m; k++)
                result.Data[i][j] += this->Data[i][k] * Two.Data[k][j];
        }
    }

    return result;
}
Matrix Matrix::operator*(const Scalar& Two) const
{
    try
    {
        return operator*(static_cast<double>(Two));
    }
    catch (OperatorException& e)
    {
        throw e;
    }
}
Matrix Matrix::operator*(double Two) const
{
    if (!this->IsValid())
        throw OperatorException('*', this->GetTypeString(), "(Scalar)", "Empty Matrix.");

    Matrix result(this->m, this->n);
    for (unsigned i = 0; i < result.m && result.Data; i++)
        for (unsigned j = 0; j < result.n; j++)
            result.Data[i][j] *= Two;

    return result;
}
Matrix Matrix::operator/(const Scalar& Two) const
{
    try
    {
        return operator/(static_cast<double>(Two));
    }
    catch (OperatorException& e)
    {
        throw e;
    }
}
Matrix Matrix::operator/(double Two) const
{
    if (!this->IsValid())
        throw OperatorException('*', this->GetTypeString(), "(Scalar)", "Empty Matrix.");

    Matrix result(this->m, this->n);
    for (unsigned i = 0; i < result.m && result.Data; i++)
        for (unsigned j = 0; j < result.n; j++)
            result.Data[i][j] *= Two;

    return result;
}
[[nodiscard]] Matrix Matrix::Pow(const Scalar& Two) const
{
    try
    {
        return this->Pow(static_cast<double>(Two));
    }
    catch (OperatorException& e)
    {
        throw e;
    }
    catch (std::exception& e)
    {
        throw e;
    }
}
[[nodiscard]] Matrix Matrix::Pow(double Two) const
{
    /*
     * for Pow to be valid on a matrix, the one of following criteria must be true.
     *
     * 1. Two == 0.00 -> IdentityMatrix of same dims
     * 2. Two == 1.00 -> Return same matrix
     * 3. Two > 1.00 && Two is integer && m == n -> Result of multiplication.
     */

    if (!this->IsValid())
        throw OperatorException('^', this->GetTypeString(), "(Scalar)", "Empty Matrix");

    if (Two == 0) //First case
        return Matrix::Identity(this->m, this->n);
    else if (Two == 1) //Second case
        return *this;
    else if (Two > 1 && floor(Two) == Two && this->m == this->n) //Third case
    {
        throw std::exception(); //Will finish in the Operators branch.
    }
    else //Invalid
        throw OperatorException('^', this->GetTypeString(), "(Scalar)",
                                this->m != this->n ? "Matrix is not square" :
                                Two < 0 ?                 "Operand is not a positive number" :
                                                          "Operand is not a whole number");
}

bool operator==(const Matrix& One, const Matrix& Two)
{
    bool Return = true;
    if (One.Columns() != Two.Columns() || One.Rows() != Two.Rows())
        return false;

    for (unsigned int i = 0; i < One.Rows(); i++)
        for (unsigned int j = 0; j < One.Columns(); j++)
            if (One[i][j] != Two[i][j])
                return false;

    return true;
}
bool operator!=(const Matrix& One, const Matrix& Two)
{
    return !(One == Two);
}
#include "Matrix.h"

#include "MathVector.h"
#include "Scalar.h"

#include <random>
#include <utility>

Matrix::Matrix(unsigned int Rows, unsigned int Columns, double Value) noexcept
{
    Allocate(Rows, Columns, Value);
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
    Allocate(Other.Rows(), Other.Columns());

    for (unsigned int i = 0; i < m; i++)
        for (unsigned int j = 0; j < n; j++)
            Data[i][j] = std::exchange(Other.Data[i][j], 0);
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
    if (Data == Other.Data) //Self Assignment
        return *this;

    Allocate(Other.Rows(), Other.Columns());

    for (unsigned int i = 0; i < m; i++)
        for (unsigned int j = 0; j < n; j++)
            Data[i][j] = std::exchange(Other.Data[i][j], 0);

    return *this;
}

void Matrix::Allocate(unsigned int NewRows, unsigned int NewColumns, double Value)
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
void Matrix::DeAllocate()
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
        m = 0;
        n = 0;
    }
}

Matrix Matrix::ErrorMatrix()
{
    return {0, 0};
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

Matrix Matrix::operator|(const Matrix& Two) const
{
    if (!Data || !Two.Data)
        throw std::logic_error("You cannot augment a matrix that is empty.");

    if (m != Two.n)
        throw std::logic_error("The dimensions for the matrices are not compatible for augmentation.");

    unsigned int OneRows = m, OneColumns = n, TwoColumns = Two.n;
    Matrix Return(OneRows, OneColumns + TwoColumns);

    if (!Return.Data)
        throw std::exception();

    for (unsigned int i = 0; i < OneRows; i++)
    {
        for (unsigned int j = 0; j < OneColumns; j++)
            Return.Data[i][j] = Two.Data[i][j];
        for (unsigned int j = OneColumns, jp = 0; j < OneColumns + TwoColumns; j++, jp++)
            Return.Data[i][j] = Two.Data[i][jp];
    }

    return Return;
}
VariableType* Matrix::operator+(const VariableType& Two) const noexcept
{

}
VariableType* Matrix::operator-(const VariableType& Two) const noexcept
{

}
VariableType* Matrix::operator*(const VariableType& Two) const noexcept
{
    if (!this->Data)
        return nullptr;

    try
    {
        switch (Two.GetType())
        {
            case VT_Scalar:
            {
                const auto& sca = dynamic_cast<const Scalar&>(Two);
                auto fac = static_cast<double>(sca);

                auto* result = new Matrix(*this);
                if (!result->Data)
                    return nullptr;

                for (unsigned i = 0; i < result->m; i++)
                    for (unsigned j = 0; j < result->n; j++)
                        result->Data[i][j] *= fac;

                return result;
            }
            case VT_Matrix:
            {
                const auto& vec = dynamic_cast<const MathVector&>(Two);

                if (vec.Dim() != n)
                    return nullptr;

                MathVector result(m);
                for (unsigned int j = 0; j < n; j++)
                {
                    MathVector CurrentCol(m, 0);
                    for (unsigned int i = 0; i < m; i++)
                        CurrentCol[i] = Data[i][j];

                    result += CurrentCol * Scalar(vec[j]);
                }

                return resul;
            }
            case VT_Vector:
            {

            }
            default:
                return nullptr;
        }
    }
    catch (std::bad_cast& e)
    {
        return nullptr;
    }
}
VariableType* Matrix::operator/(const VariableType& Two) const noexcept
{

}
VariableType* Matrix::Pow(const VariableType& Two) const noexcept
{

}

bool Matrix::operator==(const VariableType& two) const noexcept
{

}
bool Matrix::operator!=(const VariableType& two) const noexcept
{

}

std::ostream& operator<<(std::ostream& out) const noexcept
{

}
Matrix Matrix::operator*(const Matrix& B) const noexcept
{

    if (!A.IsValid() || !B.IsValid() || A.Columns() != B.Rows())
        return Matrix::ErrorMatrix();

    unsigned int R = A.Rows(), C = B.Columns();
    unsigned int R1 = R, C1 = A.Columns(), R2 = B.Rows(), C2 = C;
    Matrix Return(R, C);
    if (!Return.Data)
        throw std::logic_error("")

    for (unsigned int i = 0; i < R1; i++)
    {
        for (unsigned int j = 0; j < C2; j++)
        {
            Return[i][j] = 0;

            for (unsigned int k = 0; k < R2; k++)
                Return[i][j] += A[i][k] * B[k][j];
        }
    }

    return Return;
}
MathVector operator*(const Matrix& One, const MathVector& Two)
{
    if (Two.Dim() != One.Columns())
        return MathVector::ErrorVector();

    unsigned int Cols = One.Columns(), Rows = One.Rows();
    MathVector Return(Rows);
    for (unsigned int j = 0; j < Cols; j++)
    {
        MathVector CurrentCol(Rows, 0);
        for (unsigned int i = 0; i < Rows; i++)
            CurrentCol[i] = One[i][j];

        Return += Two[j] * CurrentCol;
    }

    return Return;
}
Matrix operator*(double One, const Matrix& Two)
{
    unsigned int Rows = Two.Rows(), Columns = Two.Columns();
    Matrix Return(Rows, Columns);

    for (unsigned int i = 0; i < Rows; i++)
        for (unsigned int j = 0; j < Columns; j++)
            Return[i][j] = Two[i][j] * One;

    return Return;
}
Matrix& Matrix::operator*=(double Val)
{
    for (unsigned int i = 0; i < m; i++)
        for (unsigned int j = 0; j < n; j++)
            Data[i][j] *= Val;

    return *this;
}
Matrix operator*(const Matrix& One, double Two)
{
    return Two * One;
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

std::ostream& operator<<(std::ostream& out, const Matrix& Data)
{
    //TODO: Fix output margins for columns with differing size.
    auto Width = [](double Data) -> int
    {
        int Return = 0;
        if (Data < 0)
            Return++;

        if (Data == 0)
            return Return + 1;

        int Red = static_cast<int>(Data);
        while (Red != 0)
        {
            Return++;
            Red /= 10;
        }
        return Return;
    };

    unsigned int Rows = Data.Rows(), Columns = Data.Columns();

    bool* Negatives = new bool[Columns] { false }; //For each column, if the column contains any number of negative numbers, then that column's value is true.
    bool* LongestIsNegative = new bool[Columns] { false }; //If the longest value (see next array) is a negative value, then this is set to true for later use.
    int* Longest = new int[Columns] { 0 }; //The length of the longest number in each column.
    int* LongestWoNeg = new int[Columns] { 0 };
    for (unsigned int j = 0; j < Columns; j++)
    {
        int TempLong = 0;
        int TempLongWoNeg = 0;
        bool TempNeg = false;
        for (unsigned int i = 0; i < Rows; i++)
        {
            if (Data[i][j] < 0)
                Negatives[j] = true;

            int EvalWidth = Width(Data[i][j]);
            if (EvalWidth > TempLong)
            {
                TempLong = EvalWidth;
                TempNeg = Data[i][j] < 0;
            }
            if (Negatives[j])
                if (EvalWidth - 1 > TempLongWoNeg)
                    TempLongWoNeg = EvalWidth - 1;
        }
        Longest[j] = TempLong;
        LongestIsNegative[j] = TempNeg;
        LongestWoNeg[j] = TempLongWoNeg;
    }

    for (unsigned int i = 0; i < Rows; i++)
    {
        for (unsigned int j = 0; j < Columns; j++)
        {
            if (j == 0) //The first row is always just one space to the next. This is because the next code block uses the previous column's data (there is no previous column)
            {
                if (Negatives[0] && Data[i][j] >= 0)
                    out << ' ';
                else if (Negatives[0] && Data[i][j] < 0)
                    out << '-';
                out << abs(Data[i][j]);
                continue;
            }

            //if (Negatives[j] && Data[i][j] >= 0) //if the current number is not negative and this column contains a negative number, it must be shifted forward one digit.
            //	out << ' ';

            int PastWidth = 0; //The width of the previous column to insert spaces.
            int PastElemWidth = Width(Data[i][j - 1]); //The length of the previous matrix.
            if (Negatives[j])
                PastWidth++;

            if (PastElemWidth == Longest[j - 1]) //If the current row contains the longest value for the last item, then the math is different.
                PastWidth += 0;
            else
                PastWidth += LongestWoNeg[j - 1] - 1; //Elsewise, the current row is just the distance from the longest of that column.

            for (int k = 0; k < PastWidth; k++) //Inserts the spaces
                out << ' ';

            if (Negatives[j] && Data[i][j] >= 0)
                out << ' ' << Data[i][j];
            else if (Negatives[j] && Data[i][j] < 0)
                out << '-' << -1 * Data[i][j];
            else
                out << ' ' << Data[i][j];
        }
        out << std::endl;
    }

    delete[] Longest;
    delete[] LongestWoNeg;
    delete[] Negatives;
    delete[] LongestIsNegative;

    return out;
}
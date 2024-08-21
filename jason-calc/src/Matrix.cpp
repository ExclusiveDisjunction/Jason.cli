#include "include/Numerics/Matrix.h"

#include "include/Numerics/MathVector.h"
#include "include/Numerics/Scalar.h"

#include <random>
#include <utility>

Matrix::Matrix() : m(0), n(0), Data(nullptr)
{
    DeAllocate();
}
Matrix::Matrix(unsigned int Rows, unsigned int Columns) noexcept : Matrix()
{
    Allocate(Rows, Columns, 0);
}
[[maybe_unused]] Matrix::Matrix(const MathVector& in) : Matrix()
{
    if (!in.IsValid())
        DeAllocate();
    else
    {
        Allocate(in.Dim(), 1);
        for (unsigned i = 0; i < this->m; i++)
            this->Data[i][0] = in[i];
    }
}
Matrix::Matrix(std::istream& in)  : Matrix()
{
    std::string header;
    in >> header;
    if (header != "MAT" || !in)
        throw std::logic_error("Cannot construct a matrix from this string.");

    in >> this->m;
    in >> this->n;

    if (this->m == 0 || this->n == 0)
        DeAllocate();
    else
    {
        Allocate(this->m, this->n);
        for (unsigned i = 0; i < this->m; i++)
        {
            for (unsigned j = 0; j < this->n; j++)
            {
                if (!in)
                    throw std::logic_error("Not enough numbers to match the dimensions.");

                in >> this->Data[i][j];
            }
        }
    }
}
Matrix::Matrix(const Matrix& Other) noexcept  : Matrix()
{
    Allocate(Other.Rows(), Other.Columns());

    for (unsigned int i = 0; i < m; i++)
        for (unsigned int j = 0; j < n; j++)
            Data[i][j] = Other.Data[i][j];
}
Matrix::Matrix(Matrix&& Other) noexcept  : Matrix()
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
    return {};
}
Matrix Matrix::Identity(unsigned int Size)
{
    return Identity(Size, Size);
}
Matrix Matrix::Identity(unsigned int Rows, unsigned int Cols)
{
    Matrix result(Rows, Cols);
    for (unsigned i = 0; i < Rows && i < Cols; i++)
        result[i][i] = 1;

    return result;
}
[[maybe_unused]] Matrix Matrix::RandomMatrix(unsigned int Rows, unsigned int Columns, bool Integers)
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
    auto* Return = new Matrix();
    Return->Data = std::exchange(this->Data, nullptr);
    Return->m = std::exchange(this->m, 0);
    Return->n = std::exchange(this->n, 0);

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
[[maybe_unused]] Matrix* Matrix::FromSterilize(const std::string& sterilized)
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

[[maybe_unused]] double Matrix::Determinant() const
{
    if (this->m != this->n)
        throw std::logic_error("The matrix must be square");

    /*
     * To do the determinant, we will have to get the minors of the matrix, and therefore requires the MatrixMinor.
     * Will be done with the addition of the matrix branch.
     */
    return 0;
}

[[maybe_unused]] Matrix Matrix::Invert() const
{
    if (Rows() != Columns())
        throw std::logic_error("The matrix must be square");

    Matrix Id = Identity(m);
    Matrix Aug = Id | *this;
    Aug.ReducedRowEchelonForm();

    Matrix Left = Aug.Extract(0, 0, m, m), Right = Aug.Extract(0, 4, m, m);
    if (Right != Id)
        return ErrorMatrix();
    else
        return Left;
}
[[maybe_unused]] Matrix Matrix::Transpose() const
{
    Matrix Return(m, n);
    if (!Return.Data)
        throw std::logic_error("Cannot transpose an empty matrix.");

    for (unsigned int i = 0; i < m; i++)
        for (unsigned int j = 0; j < n; j++)
            Return.Data[j][i] = Data[i][j];

    return Return;
}

void Matrix::RowEchelonForm()
{
    if (!this->IsValid())
        return;

    unsigned rows = this->m;
    unsigned cols = this->n;
    unsigned currentRow = 0;

    for (unsigned current_col = 0; current_col < cols; current_col++)
    {
        unsigned pivot_row = current_col;
        while (pivot_row < rows && this->Data[pivot_row][current_col] == 0)
            pivot_row++;

        if (pivot_row < rows)
        {
            this->RowSwap(currentRow, pivot_row);

            double pivot_value = this->Data[currentRow][current_col];
            for (unsigned col = current_col; col < cols; col++)
                this->Data[currentRow][col] /= pivot_value;

            for (unsigned row = currentRow + 1; row < rows; row++)
            {
                double mul = this->Data[row][current_col];
                for (unsigned col = current_col; col < cols; col++)
                    this->Data[row][col] -= mul * this->Data[currentRow][col];
            }

            currentRow++;
        }
    }
}
void Matrix::ReducedRowEchelonForm()
{
    this->RowEchelonForm();

    unsigned rows = this->m;
    unsigned cols = this->n;
    unsigned lead = 0;

    for (unsigned r = 0; r < cols; r++)
    {
        if (lead >= cols)
            break;

        unsigned i = r;
        while (this->Data[i][lead] == 0)
        {
            i++;
            if (i == rows)
            {
                i = r;
                lead++;
                if (lead == cols)
                    break;
            }
        }

        if (lead < cols)
        {
            this->RowSwap(i, r);
            double divisor = this->Data[r][lead];
            if (divisor == 0)
            {
                for (unsigned j = 0; j < cols; j++)
                    this->Data[r][j] /= divisor;
            }

            for (unsigned k = 0; k < rows; k++)
            {
                if (k != r)
                {
                    double factor = this->Data[k][lead];
                    for (unsigned j = 0; j < cols; j++)
                        this->Data[k][j] = factor * this->Data[r][j];
                }
            }
        }

        lead++;
    }

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

std::vector<std::pair<bool, unsigned long>> Matrix::GetColumnWidthSchematic() const
{
    /*
        This algorithm will find the largest width of each column, and store the result in the result value.

        For instance:

        [4 3 1 2]
        [33 4 1 -2]
        [5 -44.3 2 1]

        Which evaluates to:
        [(false, 2), (true, 5), (false, 1), (true, 2)]

        Which can be used to re-format the matrix when printing to be:
        [4   3    1  2]
        [33  4    1 -2]
        [5  -44.3 2  1]
    */
    std::vector<std::pair<bool, unsigned long>> result;

    for (unsigned j = 0; j < this->n; j++)
    {
        bool negative_found = false;
        unsigned long largest_num = 0;
        for (unsigned i = 0; i < this->m; i++)
        {
            double curr = this->Data[i][j];
            if (curr < 0)
                negative_found = true;

            unsigned long as_str_l;
            {
                std::stringstream temp;
                temp << curr;
                as_str_l = static_cast<unsigned long>(temp.str().length());

                if (curr < 0) //Removes negative from the computation.
                    as_str_l--;
            }

            largest_num = std::max(as_str_l, largest_num);
        }

        result.emplace_back(negative_found, largest_num);
    }

    return result;
}
bool Matrix::GetRowString(std::ostream& out, unsigned row, std::vector<std::pair<bool, unsigned long>>& schema, char open, char close) const
{
    if (row > this->m)
        return false;

    /*
        Convert the number to a string
        If the length of that string is less than the maximum of the column, then add extra spaces to the end till it meets the maximum.
        If there is a negative in that column, and the current number is not negative, add one to the beginning of the string, and remove one from the end. If it is negative, do not add any extra spaces.
        Repeat this for each number for each column and row, and put a space between columns.
    */

    out << open << ' ';
    const double* host = this->Data[row];
    for (unsigned i = 0; i < this->n; i++)
    {
        const auto& this_schema = schema[i];
        bool has_negative = this_schema.first;
        unsigned long width = this_schema.second;

        double curr = host[i];
        if (curr >= 0 && has_negative)
            out << ' ';

        std::string curr_str;
        {
            std::stringstream temp;
            temp << curr;
            curr_str = temp.str();
        }


        if (((has_negative && curr_str.length() < width + 1) || curr_str.length() < width))
        {
            unsigned long diff = width - (curr_str.length()) + (curr < 0 ? 1 : 0);
            std::string space_str;
            //if (has_negative && i != this->n - 1) //If not in last row
            //   diff += (curr < 0 ? 1 : 0); //If negative, add one extra space after.

            for (unsigned t = 0; t < diff; t++)
                space_str += ' ';

            curr_str += space_str;
        }
        out << curr_str << ' ';
    }

    out << close;
    return static_cast<bool>(out); //If out.bad(), this returns false.
}
void Matrix::Print(std::ostream& out) const noexcept
{
    if (!this->IsValid())
    {
        out << "[ ]";
    }

    auto schema = this->GetColumnWidthSchematic();

    if (this->m == 1)
    {
        if (!this->GetRowString(out, 0, schema, '[', ']'))
            std::cerr << "FAILED TO PRINT LINE 0" << std::endl;
        else
            out << '\n';
    }
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

            if (!this->GetRowString(out, i, schema, open, close)) //Returns false if could not print properly.
                std::cerr << "FAILED TO PRINT LINE " << i << std::endl;
            else
                out << '\n';
        }
    }


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
    Matrix result(*this);
    result += Two;
    return result;
}
Matrix Matrix::operator-(const Matrix& Two) const
{
    Matrix result(*this);
    result -= Two;
    return result;
}
Matrix Matrix::operator*(const Matrix& Two) const
{
    Matrix result(*this);
    result.operator*=(Two);
    return result;
}

Matrix& Matrix::operator+=(const Matrix& Two)
{
    if (!this->IsValid() || !Two.IsValid())
        throw OperatorException('+', this->GetTypeString(), Two.GetTypeString(), "Empty Matrix");

    if (this->m != Two.m || this->n != Two.n)
        throw OperatorException('+', this->GetTypeString(), Two.GetTypeString(), "Dimension Mismatch");

    for (unsigned i = 0; i < this->m && this->Data; i++)
        for (unsigned j = 0; j < this->n; j++)
            this->Data[i][j] += Two.Data[i][j];

    return *this;
}
Matrix& Matrix::operator-=(const Matrix& Two)
{
    if (!this->IsValid() || !Two.IsValid())
        throw OperatorException('-', this->GetTypeString(), Two.GetTypeString(), "Empty Matrix");

    if (this->m != Two.m || this->n != Two.n)
        throw OperatorException('-', this->GetTypeString(), Two.GetTypeString(), "Dimension Mismatch");

    for (unsigned i = 0; i < this->m && this->Data; i++)
        for (unsigned j = 0; j < this->n; j++)
            this->Data[i][j] -= Two.Data[i][j];

    return *this;
}
Matrix& Matrix::operator*=(const Matrix& Two)
{
    if (!this->IsValid() || !Two.IsValid())
        throw OperatorException('*', this->GetTypeString(), Two.GetTypeString(), "Empty Matrix");

    if (this->n != Two.m)
        throw OperatorException('*', this->GetTypeString(), Two.GetTypeString(), "Dimension mismatch");

    unsigned r = this->m, c = Two.n;

    for (unsigned i = 0; i < r && this->Data; i++)
    {
        for (unsigned j = 0; j < c; j++)
        {
            double calc = 0; //Since this is a matrix multiplication, I cannot reset the value at Data[i][j] since it will interfere with the calculation.
            for (unsigned k = 0; k < Two.m; k++)
                calc += this->Data[i][k] * Two.Data[k][j];

            this->Data[i][j] = calc;
        }
    }

    return *this;
}

[[nodiscard]] Matrix Matrix::Pow(unsigned long long Two) const
{
    if (!this->IsValid())
        throw OperatorException('^', this->GetTypeString(), "(Scalar)", "Empty Matrix");

    if (Two == 0)
        return Matrix::Identity(this->m, this->n);
    else if (Two == 1)
        return *this;
    else
    {
        if (this->m != this->n)
            throw OperatorException('^', this->GetTypeString(), "(Scalar)", "Non-square matrix does not support powers greater than 1");

        Matrix result(*this);
        for (unsigned long long i = 1; i < Two; i++)
            result *= *this;

        return result;
    }
}

std::ostream& operator<<(std::ostream& out, const MatrixSingleLinePrint& Obj)
{
    const auto& Target = Obj.Target;

    if (!Target.IsValid())
        out << "[ ]";
    else
    {
        out << '[';
        unsigned r = Target.Rows(), c = Target.Columns();

        try
        {
            for (unsigned i = 0; i < r; i++)
            {
                for (unsigned j = 0; j < c; j++)
                    out << (j == 0 ? " " : ", ") << Target[i][j];

                if (i != r - 1)
                    out << ";";
            }
        }
        catch (std::logic_error& e) //This should not ever happen, but it is here as a safe-gaurd.
        {
            out << "Error while printing!\n";
        }

        out << ']';
    }

    return out;
}
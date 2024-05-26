#include "Matrix.h"

namespace Math
{
	Matrix::Matrix(unsigned int Rows, unsigned int Columns, double Value)
	{
		if (Rows <= 0 || Columns <= 0)
			throw std::exception("The rows or columns cannot be zero or less.");

		Allocate(Rows, Columns, Value);
	}
	Matrix::Matrix(const Matrix& Other) noexcept
	{
		Allocate(Other.Rows(), Other.Columns());
		for (unsigned int i = 0; i < _Rows; i++)
			for (unsigned int j = 0; j < _Columns; j++)
				Data[i][j] = Other.Data[i][j];
	}
	Matrix::Matrix(Matrix&& Other) noexcept
	{
		if (Other.Rows() != _Rows || Other.Columns() != _Columns)
			Allocate(Other.Rows(), Other.Columns());

		for (unsigned int i = 0; i < _Rows; i++)
		{
			for (unsigned int j = 0; j < _Columns; j++)
			{
				Data[i][j] = Other.Data[i][j];
				Other.Data[i][j] = 0;
			}
		}
	}
	Matrix::~Matrix()
	{
		DeAllocate();
	}

	void Matrix::Allocate(unsigned int NewRows, unsigned int NewColumns, double Value)
	{
		DeAllocate();

		Data = new double* [NewRows];
		for (unsigned int i = 0; i < NewRows; i++)
			Data[i] = new double[NewColumns] { Value };

		_Rows = NewRows;
		_Columns = NewColumns;
	}
	void Matrix::DeAllocate()
	{
		if (Data)
		{
			for (unsigned int i = 0; i < _Rows; i++)
			{
				delete[] Data[i];
				Data[i] = nullptr;
			}
			delete[] Data;

			Data = nullptr;
		}

		_Rows = 0;
		_Columns = 0;
	}

	Matrix Matrix::ErrorMatrix()
	{
		Matrix Return(1, 1);
		Return.DeAllocate();

		return Return;
	}
	Matrix Matrix::Identity(unsigned int Size)
	{
		Matrix Return(Size, Size, 0);
		for (unsigned int i = 0; i < Size; i++)
			Return[i][i] = 1;

		return Return;
	}
	Matrix Matrix::RandomMatrix(unsigned int Rows, unsigned int Columns)
	{
		Matrix Return(Rows, Columns);
		int upper = 10, lower = -10;
		srand(static_cast<unsigned int>(time(0)));
		for (unsigned int i = 0; i < Rows; i++)
			for (unsigned int j = 0; j < Columns; j++)
				Return.Data[i][j] = static_cast<double>((rand() % (upper - lower + 1)) + lower);

		return Return;
	}

	double* Matrix::operator[](unsigned int Row) const
	{
		if (Row > _Rows)
			return nullptr;

		return Data[Row];
	}

	Matrix& Matrix::operator=(const Matrix& Other) noexcept
	{
		if (Other.Rows() != _Rows || Other.Columns() != _Columns)
			Allocate(Other.Rows(), Other.Columns());

		for (unsigned int i = 0; i < _Rows; i++)
			for (unsigned int j = 0; j < _Columns; j++)
				Data[i][j] = Other.Data[i][j];

		return *this;
	}
	Matrix& Matrix::operator=(Matrix&& Other) noexcept
	{
		if (Other.Rows() != _Rows || Other.Columns() != _Columns)
			Allocate(Other.Rows(), Other.Columns());

		for (unsigned int i = 0; i < _Rows; i++)
		{
			for (unsigned int j = 0; j < _Columns; j++)
			{
				Data[i][j] = Other.Data[i][j];
				Other.Data[i][j] = 0;
			}
		}

		return *this;
	}

	Matrix Matrix::Extract(unsigned int Starti, unsigned int Startj, unsigned int RowCount, unsigned int ColumnCount)
	{
		if ((Starti + RowCount - 1) > _Rows || (Startj + ColumnCount - 1) > _Columns)
			throw std::exception("The index was out of range for that size.");

		Matrix Return(RowCount, ColumnCount);
		for (unsigned int i = Starti, ip = 0; i < Starti + RowCount - 1; i++, ip++)
			for (unsigned int j = Startj, jp = 0; j < Startj + ColumnCount - 1; j++, jp++)
				Return.Data[ip][jp] = Data[i][j];

		return Return;
	}

	void Matrix::RowSwap(unsigned int OrigRow, unsigned int NewRow)
	{
		if (OrigRow == NewRow)
			return;

		for (unsigned int i = 0; i < _Columns; i++)
		{
			double Temp = Data[OrigRow][i];
			Data[OrigRow][i] = Data[NewRow][i];
			Data[NewRow][i] = Temp;
		}
	}
	void Matrix::RowAdd(unsigned int OrigRow, double Fac, unsigned int TargetRow)
	{
		if (Fac == 0)
			throw std::exception("The factor of multiplication cannot be zero.");

		for (unsigned int j = 0; j < _Columns; j++)
			Data[TargetRow][j] += Data[OrigRow][j] * Fac;
	}

	double Matrix::Determinant() const
	{
		if (Rows() != Columns())
			return INFINITY;

		return 0; //TODO: Finish Determinant
	}

	Matrix Matrix::Invert() const
	{
		if (Rows() != Columns())
			return ErrorMatrix();

		Matrix Id = Identity(_Rows);
		Matrix Aug = Id | *this;
		Aug.RREF();

		Matrix Left = Aug.Extract(0, 0, _Rows, _Rows), Right = Aug.Extract(0, 4, _Rows, _Rows);
		if (Right != Id)
			return ErrorMatrix();
		else
			return Left;
	}
	Matrix Matrix::Transpose() const
	{
		Matrix Return(_Rows, _Columns);
		for (unsigned int i = 0; i < _Rows; i++)
			for (unsigned int j = 0; j < _Columns; j++)
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
			int i = r;
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

	Matrix operator|(const Matrix& One, const Matrix& Two)
	{
		if (One.Rows() != Two.Rows())
			throw std::exception("The dimensions for the matrices are not capatible for augmentation.");

		unsigned int OneRows = One.Rows(), OneColumns = One.Columns(), TwoColumns = Two.Columns();
		Matrix Return(OneRows, OneColumns + TwoColumns);
		for (unsigned int i = 0; i < OneRows; i++)
		{
			for (unsigned int j = 0; j < One.Columns(); j++)
				Return[i][j] = One[i][j];
			for (unsigned int j = OneColumns, jp = 0; j < OneColumns + TwoColumns; j++, jp++)
				Return[i][j] = Two[i][jp];
		}

		return Return;
	}
	Matrix operator*(const Matrix& A, const Matrix& B)
	{
		if (!A.IsValid() || !B.IsValid() || A.Columns() != B.Rows())
			return Matrix::ErrorMatrix();

		unsigned int R = A.Rows(), C = B.Columns();
		unsigned int R1 = R, C1 = A.Columns(), R2 = B.Rows(), C2 = C;
		Matrix Return(R, C);

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
		for (unsigned int i = 0; i < _Rows; i++)
			for (unsigned int j = 0; j < _Columns; j++)
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
}
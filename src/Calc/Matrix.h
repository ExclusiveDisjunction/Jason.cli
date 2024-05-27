#pragma once

#include "StdCalc.h"
#include "MathVector.h"

	/// <summary>
	/// Represents a rectangular arrangment of numbers for calculations, given some row and column definition. 
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
        /*
         * For borrows, I think we should leave the Extract method to be a clone of data, but also introduce a
         * SliceMatrix type. This will borrow data from a specified matrix, and will enable the matrix to share it's data
         * without being copied. This is useful for finding the determinant.
         *
         * The slices will have a specified 'range' (i, j) -> (m, n) that will say what it 'looks at'. For the full matrix,
         * (0, 0) -> (UINT_MAX, UINT_MAX) is used.
         *
         * If the origional matrix is:
         * 1. Deleted - The slices loose their reference.
         * 2.
         */

		void RowSwap(unsigned int OrigRow, unsigned int NewRow);
		void RowAdd(unsigned int OrigRow, double Fac, unsigned int TargetRow);

		[[nodiscard]] double Determinant() const;

		[[nodiscard]] Matrix Invert() const;
		[[nodiscard]] Matrix Transpose() const;

		void REF();
		void RREF();

        Matrix operator|(const Matrix& Two) const;
        VariableType* operator+(const VariableType& Two) const noexcept override;
        VariableType* operator-(const VariableType& Two) const noexcept override;
        VariableType* operator*(const VariableType& Two) const noexcept override;
        VariableType* Pow(const VariableType& Two) const noexcept override;

        bool operator==(const VariableType& two) const noexcept override;
        bool operator!=(const VariableType& two) const noexcept override;

        std::ostream& operator<<(std::ostream& out) const noexcept override;
	};
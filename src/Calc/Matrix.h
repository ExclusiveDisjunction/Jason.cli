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
		double** Data;
		unsigned int _Rows;
		unsigned int _Columns;

		void Allocate(unsigned int NewRows, unsigned int NewColumns, double Value = 0);
		void DeAllocate();

	public:
		Matrix(unsigned int Rows, unsigned int Columns, double Value = 0);
        Matrix(std::istream& in);
		Matrix(const Matrix& Other) noexcept;
		Matrix(Matrix&& Other) noexcept;
		~Matrix();

		[[nodiscard]] unsigned int Rows() const { return _Rows; }
		[[nodiscard]] unsigned int Columns() const { return _Columns; }
		[[nodiscard]] bool IsValid() const { return _Rows != 0 && _Columns != 0; }

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
            ss << "(Matrix:" << _Rows << "x" << _Columns << ")";
            return ss.str();
        }

		static Matrix ErrorMatrix();
		static Matrix Identity(unsigned int Size);
		static Matrix RandomMatrix(unsigned int Rows, unsigned int Columns);

		const double* operator[](unsigned int Row) const;

		Matrix& operator=(const Matrix& Other) noexcept;
		Matrix& operator=(Matrix&& Other) noexcept;

		[[nodiscard]] Matrix Extract(unsigned int Starti, unsigned int Startj, unsigned int RowCount, unsigned int ColumnCount);

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
#include "MatrixFunction.h"

#include "Polynomial.h"

namespace Math::Function
{
	MatrixFunction::MatrixFunction(unsigned int Rows, unsigned int Columns) : FunctionBase(Columns, Rows)
	{
		Fill(Rows, Columns);
	}
	MatrixFunction::MatrixFunction(const Matrix& Obj) : FunctionBase(Obj.Columns(), Obj.Rows())
	{
		Fill(Obj.Rows(), Obj.Columns());

		for (unsigned int i = 0; i < Rows; i++)
			for (unsigned int j = 0; j < Columns; j++)
				Core[j].Point[i] = Obj[i][j];
	}
	MatrixFunction::~MatrixFunction()
	{
		Clear();
	}

	void MatrixFunction::Fill(unsigned int Rows, unsigned int Columns)
	{
		Clear();

		void* raw = operator new[](Columns * sizeof(MathVector));
		Core = static_cast<MathVector*>(raw);
		for (unsigned int i = 0; i < Columns; i++)
			Core[i] = MathVector(Rows);

		this->Rows = Rows;
		this->Columns = Columns;
	}
	void MatrixFunction::Clear()
	{
		if (Core)
		{
			for (int i = static_cast<int>(Columns); i >= 0; i--)
				Core[i].~MathVector();

			operator delete[](Core);
		}

		Rows = 0;
		Columns = 0;
	}
	void MatrixFunction::ChildRemoved(FunctionBase* Child)
	{
		//Matrix functions cannot have children. 
	}

	VectorFunction* MatrixFunction::ToVectorFunction() const
	{
		VectorFunction* Return = new VectorFunction(Columns, Rows);

		for (unsigned int i = 0; i < Rows; i++)
		{
			Polynomial* Current = new Polynomial(Columns, 1);
			for (unsigned int j = 0; j < Columns; j++)
				Current->AddFunction(new Monomial(Columns, j, Core[j][i], 1));

			Return->AssignFunction(i, Current);
			Current = nullptr;
		}

		return Return;
	}

	MathVector MatrixFunction::Evaluate(const MathVector& In, bool& Exists) const
	{
		if (In.Dim() != Columns)
		{
			Exists = false;
			return MathVector::ErrorVector();
		}

		Exists = true;

		MathVector Return(Rows);
		for (unsigned int i = 0; i < Columns; i++)
			Return += In[i] * Core[i];

		return Return;
	}

	void MatrixFunction::ToMatrix(Matrix& Dest) const
	{
		Dest = Matrix(Rows, Columns);

		for (unsigned int i = 0; i < Rows; i++)
			for (unsigned int j = 0; j < Columns; j++)
				Dest[i][j] = Core[j].Point[i];
	}
	void MatrixFunction::FromMatrix(const Matrix& Obj)
	{
		if (Obj.Rows() != Rows || Obj.Columns() != Columns)
			return;

		for (unsigned int i = 0; i < Rows; i++)
			for (unsigned int j = 0; j < Columns; j++)
				Core[j].Point[i] = Obj[i][j];
	}
}
#pragma once

#include "FunctionBase.h"
#include "VectorFunction.h"

namespace Math::Function
{
	class MATH_LIB MatrixFunction : public FunctionBase
	{
	private:
		MathVector* Core;

		void Fill(unsigned int Rows, unsigned int Columns);
		void Clear();

		unsigned int Rows;
		unsigned int Columns;

	protected:
		void ChildRemoved(FunctionBase* Child) override;

	public:
		MatrixFunction(unsigned int Rows, unsigned int Columns);
		MatrixFunction(const Matrix& Obj);
		MatrixFunction(const MatrixFunction& Obj) noexcept = delete;
		MatrixFunction(MatrixFunction&& Obj) noexcept = delete;
		~MatrixFunction();

		MatrixFunction& operator=(const MatrixFunction& Obj) noexcept = delete;
		MatrixFunction& operator=(MatrixFunction&& Obj) noexcept = delete;

		VectorFunction* ToVectorFunction() const;
		unsigned int AllowedChildCount() const override { return 0; }
		bool AllowsChildAppend() const override { return false; }

		MathVector Evaluate(const MathVector& In, bool& Exists) const override;

		void ToMatrix(Matrix& Dest) const;
		void FromMatrix(const Matrix& Obj);
	};
}
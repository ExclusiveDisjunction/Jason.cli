#pragma once

#include "Matrix.h"
#include "MathVector.h"

namespace Math
{
	class MATH_LIB Linear2dTransformation
	{
	private:
		Matrix TransMat = Matrix::Identity(2);
		MathVector Transpose = MathVector(2u, 0.0);
		Matrix Inverse = Matrix::Identity(2);

	public:
		Linear2dTransformation();

		static Matrix RotationMat(double Theta);
		static Matrix InvRotationMat(double Theta);
		
		static Matrix StrechMat(double k, double l);
		static Matrix InvStrechMat(double k, double l);
		static Matrix ZoomMat(double f) { return StrechMat(f, f); }
		static Matrix InvZoomMat(double f) { return InvStrechMat(f, f); }

		static Matrix ShearMat(double k, double l);
		static Matrix InvShearMat(double k, double l);

		static Matrix ReflectionMat(MathVector Line);
		static Matrix InvReflectionMat(MathVector Line);
		
		void ApplyRotation(double Theta); //Counter Clock Wise
		void UndoRotation(double Theta);

		void ApplyStrech(double ip, double jp);
		void UndoStrech(double ip, double jp);		
		void ApplyZoom(double Fac);
		void UndoZoom(double Fac);

		void ApplyShear(double ip, double jp);
		void UndoShear(double ip, double jp);

		void ApplyReflection(MathVector l);
		void ReverseReflection(MathVector l);

		void ApplyTranspose(MathVector Vec);

		MathVector operator*(const MathVector& In) const;
	};
}
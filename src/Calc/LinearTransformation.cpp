#include "LinearTransformation.h"

namespace Math
{
	Linear2dTransformation::Linear2dTransformation()
	{

	}

	Matrix Linear2dTransformation::RotationMat(double Theta)
	{
		Matrix Return(2, 2);
		Return[0][0] = cos(Theta);
		Return[0][1] = -sin(Theta);
		Return[1][0] = sin(Theta);
		Return[1][1] = cos(Theta);

		return Return;
	}
	Matrix Linear2dTransformation::InvRotationMat(double Theta)
	{
		Matrix Return(2, 2);
		Return[0][0] = cos(Theta);
		Return[0][1] = sin(Theta);
		Return[1][0] = -sin(Theta);
		Return[1][1] = cos(Theta);

		return Return;
	}

	Matrix Linear2dTransformation::StrechMat(double k, double l)
	{
		Matrix Return(2, 2);
		Return[0][0] = k;
		Return[0][1] = 0;
		Return[1][0] = 0;
		Return[1][1] = l;

		return Return;
	}
	Matrix Linear2dTransformation::InvStrechMat(double k, double l)
	{
		Matrix Return(2, 2);
		Return[0][0] = 1/k;
		Return[0][1] = 0;
		Return[1][0] = 0;
		Return[1][1] = 1/l;

		return Return;
	}

	Matrix Linear2dTransformation::ShearMat(double k, double l)
	{
		Matrix Return(2, 2);
		Return[0][0] = 1;
		Return[0][1] = k;
		Return[1][0] = l;
		Return[1][1] = 1;

		return Return;
	}
	Matrix Linear2dTransformation::InvShearMat(double k, double l)
	{
		Matrix Return(2, 2);
		Return[0][0] = 1;
		Return[0][1] = -l;
		Return[1][0] = -k;
		Return[1][1] = 1;

		return (1/(1-k*l)) * Return;
	}

	Matrix Linear2dTransformation::ReflectionMat(MathVector Line)
	{
		if (Line.Dim() != 0)
			return Matrix::ErrorMatrix();

		Matrix Return(2, 2);
		Return[0][0] = pow(Line[0],2) - pow(Line[1],2);
		Return[0][1] = 2 * Line[0] * Line[1];
		Return[1][0] = 2 * Line[0] * Line[1];
		Return[1][1] = pow(Line[1],2) - pow(Line[0],2);

		return (1 / (pow(Line[0], 2) + pow(Line[1], 2))) * Return;
	}
	Matrix Linear2dTransformation::InvReflectionMat(MathVector Line)
	{
		if (Line.Dim() != 0)
			return Matrix::ErrorMatrix();

		Matrix Return(2, 2);
		Return[0][0] = pow(Line[0], 2) - pow(Line[1], 2);
		Return[0][1] = 2 * Line[0] * Line[1];
		Return[1][0] = 2 * Line[0] * Line[1];
		Return[1][1] = pow(Line[1], 2) - pow(Line[0], 2);

		return Return;
	}

	void Linear2dTransformation::ApplyRotation(double Theta)
	{
		TransMat = TransMat * RotationMat(Theta);
	}
	void Linear2dTransformation::UndoRotation(double Theta)
	{
		TransMat = TransMat * InvRotationMat(Theta);
	}

	void Linear2dTransformation::ApplyStrech(double ip, double jp)
	{
		TransMat = TransMat * StrechMat(ip, jp);
	}
	void Linear2dTransformation::UndoStrech(double ip, double jp)
	{
		TransMat = TransMat * InvStrechMat(ip, jp);
	}
	void Linear2dTransformation::ApplyZoom(double Fac)
	{
		ApplyStrech(Fac, Fac);
	}
	void Linear2dTransformation::UndoZoom(double Fac)
	{
		ApplyStrech(Fac, Fac);
	}

	void Linear2dTransformation::ApplyShear(double ip, double jp)
	{
		TransMat = TransMat * ShearMat(ip, jp);
	}
	void Linear2dTransformation::UndoShear(double ip, double jp)
	{
		TransMat = TransMat * InvShearMat(ip, jp);
	}

	void Linear2dTransformation::ApplyReflection(MathVector l)
	{
		if (l.Dim() != 0)
			return;

		TransMat = TransMat * ReflectionMat(l);
	}
	void Linear2dTransformation::ReverseReflection(MathVector l)
	{
		if (l.Dim() != 0)
			return;

		TransMat = TransMat * InvReflectionMat(l);
	}

	void Linear2dTransformation::ApplyTranspose(MathVector Vec)
	{
		if (Vec.Dim() != 2)
			return;

		Transpose += Vec;
	}

	MathVector Linear2dTransformation::operator*(const MathVector& In) const
	{
		return (TransMat * In) + Transpose;
	}
}
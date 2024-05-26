#include "MathVector.h"
#include "Matrix.h"

MathVector::MathVector(unsigned int Dim, double Val)
{
    if (Dim <= 0)
        throw std::logic_error("The dimension is equal to zero, and this is not allowed.");

    Allocate(Dim, Val);

    for (unsigned int i = 0; i < Dim; i++)
        Point[i] = Val;
}
MathVector::MathVector(unsigned int Dim, double* Point)
{
    if (Dim <= 0)
        throw std::logic_error("The dimension is equal to zero, and this is not allowed.");

    Allocate(Dim, 0);

    for (unsigned int i = 0; i < Dim; i++)
        this->Point[i] = Point[i];
}
MathVector::MathVector(const MathVector& Obj) noexcept
{
    if (Obj._Dim == 0 || !Obj.Point)
    {
        DeAllocate();
        return;
    }

    Allocate(Obj._Dim, 0);

    for (unsigned int i = 0; i < _Dim; i++)
        Point[i] = Obj.Point[i];
}
MathVector::MathVector(MathVector&& Obj) noexcept
{
    if (Obj._Dim == 0 || !Obj.Point)
    {
        DeAllocate();
        return;
    }

    Allocate(Obj._Dim, 0);

    for (unsigned int i = 0; i < _Dim; i++)
        Point[i] = Obj.Point[i];

    Obj.DeAllocate();
}
MathVector::~MathVector()
{
    DeAllocate();
}

MathVector& MathVector::operator=(const MathVector& Obj) noexcept
{
    if (*this == Obj)
        return *this;

    if (Obj._Dim == 0 || !Obj.Point)
    {
        DeAllocate();
        return *this;
    }

    if (Obj._Dim != _Dim)
        Allocate(Obj._Dim, 0);

    for (unsigned int i = 0; i < _Dim; i++)
        Point[i] = Obj.Point[i];

    return *this;
}
MathVector& MathVector::operator=(MathVector&& Obj) noexcept
{
    if (*this == Obj)
        return *this;

    if (Obj._Dim == 0 || !Obj.Point)
    {
        DeAllocate();
        return *this;
    }

    if (Obj._Dim != _Dim)
        Allocate(Obj._Dim, 0);

    for (unsigned int i = 0; i < _Dim; i++)
        Point[i] = Obj.Point[i];

    Obj.DeAllocate();
    return *this;
}

void MathVector::Allocate(unsigned int Dim, double Val)
{
    DeAllocate();

    Point = new double[Dim] {Val};
    _Dim = Dim;
}
void MathVector::DeAllocate()
{
    if (Point)
    {
        delete[] Point;
        Point = nullptr;
        _Dim = 0;
    }
}

MathVector MathVector::ZeroVector(unsigned int Dim)
{
    return MathVector(Dim, 0);
}
MathVector MathVector::ErrorVector()
{
    MathVector Return(1, 0);
    Return.DeAllocate();

    return Return;
}

double& MathVector::operator[](unsigned int Index) const
{
    if (Index >= _Dim)
        throw std::exception("The index provided is invalid.");

    return Point[Index];
}

double MathVector::Magnitude() const
{
    if (_Dim == 1)
        return Point[0];

    if (_Dim <= 0)
        throw std::exception("A magnitude cannot be measured on a dimensionless object.");

    double Sum = 0;
    for (unsigned int i = 0; i < _Dim; i++)
        Sum += Point[i] * Point[i];

    return sqrt(Sum);
}
double MathVector::Angle() const
{
    if (_Dim <= 1)
        throw std::exception("Cannot measure the angle of a scalar or lower rank mathematical object.");

    return atan(Magnitude());
}

MathVector MathVector::CrossProduct(const MathVector& One, const MathVector& Two)
{
    if (Two.Dim() != One.Dim())
        return ErrorVector();

    MathVector A, B;
    switch (One.Dim())
    {
    case 1:
        return One[0] * Two[0];
    case 2:
        A = MathVector(One[0], One[1], 0);
        B = MathVector(Two[0], Two[1], 0);
    case 3:
        A = One;
        B = Two;
    default:
        return ErrorVector();
    }

    return MathVector((A[1] * B[2]) - (A[2] * B[1]), (A[2] * B[0]) - (A[0] * B[2]), (A[0] * B[1]) - (A[1] * B[0])); //Uses the cross product equation.
}
double MathVector::DotProduct(const MathVector& One, const MathVector& Two)
{
    if (One._Dim != Two._Dim)
        return std::numeric_limits<double>::infinity();

    double Return = 0.0;
    for (unsigned int i = 0; i < One._Dim; i++)
        Return += One[i] * Two[i];

    return Return;
}

MathVector::operator double() const
{
    return Magnitude();
}
MathVector::operator Matrix() const
{
    Matrix Return(_Dim, 1);

    for (unsigned int i = 0; i < _Dim; i++)
        Return[i][0] = Point[i];

    return Return;
}

MathVector operator+(const MathVector& One, const MathVector& Two)
{
    if (One.Dim() != Two.Dim())
        return MathVector::ErrorVector();

    unsigned int Dim = One.Dim();
    MathVector Return(Dim, 0);
    for (unsigned int i = 0; i < Dim; i++)
        Return[i] = One[i] + Two[i];

    return Return;
}
MathVector& MathVector::operator+=(const MathVector& Obj)
{
    if (Obj._Dim != _Dim)
        throw std::exception("The dimensions do not match.");

    for (unsigned int i = 0; i < _Dim; i++)
        Point[i] += Obj[i];

    return *this;
}
MathVector operator-(const MathVector& One, const MathVector& Two)
{
    if (One.Dim() != Two.Dim())
        return MathVector::ErrorVector();

    unsigned int Dim = One.Dim();
    MathVector Return(Dim, 0);
    for (unsigned int i = 0; i < Dim; i++)
        Return[i] = One[i] - Two[i];

    return Return;
}
MathVector& MathVector::operator-=(const MathVector& Obj)
{
    if (Obj._Dim != _Dim)
        throw std::exception("The dimensions do not match.");

    for (unsigned int i = 0; i < _Dim; i++)
        Point[i] -= Obj[i];

    return *this;
}
MathVector operator*(const MathVector& One, double Two)
{
    unsigned int Dim = One.Dim();
    MathVector Return(Dim, 0);
    for (unsigned int i = 0; i < Dim; i++)
        Return[i] = One[i] * Two;

    return Return;
}
MathVector operator*(double One, const MathVector& Two) { return Two * One; }
MathVector& MathVector::operator*=(double Obj)
{
    for (unsigned int i = 0; i < _Dim; i++)
        Point[i] *= Obj;

    return *this;
}
MathVector operator/(const MathVector& One, double Two)
{
    unsigned int Dim = One.Dim();
    MathVector Return(Dim, 0);
    for (unsigned int i = 0; i < Dim; i++)
        Return[i] = One[i] / Two;

    return Return;
}
MathVector& MathVector::operator/=(double Obj)
{
    for (unsigned int i = 0; i < _Dim; i++)
        Point[i] /= Obj;

    return *this;
}

bool operator==(const MathVector& One, const MathVector& Two)
{
    if (One.Dim() != Two.Dim())
        return false;

    unsigned int Dim = One.Dim(), i;
    for (i = 0; i < Dim; i++)
        if (One[i] != Two[i])
            return false;

    return true;
}
bool operator!=(const MathVector& One, const MathVector& Two)
{
    return !(One == Two);
}
#include "MathVector.h"
#include "Matrix.h"
#include "Scalar.h"

MathVector::MathVector(unsigned int Dim, double Val)
{
    if (Dim <= 0)
        throw std::logic_error("The dimension is equal to zero, and this is not allowed.");

    Allocate(Dim, Val);

    for (unsigned int i = 0; i < Dim; i++)
        Point[i] = Val;
}
MathVector::MathVector(unsigned int Dim, const double* Point)
{
    if (Dim <= 0)
        throw std::logic_error("The dimension is equal to zero, and this is not allowed.");

    Allocate(Dim, 0);

    for (unsigned int i = 0; i < Dim; i++)
        this->Point[i] = Point[i];
}
MathVector::MathVector(std::istream &in)
{
    std::string header;
    in >> header;
    if (header != "VEC")
        throw std::logic_error("Cannot construct vector from stream because the header is not the stream.");

    in >> this->d;
    if (d < 0)
        throw std::logic_error("Dimension is negative.");
    else if (d == 0)
    {
        DeAllocate();
        return;
    }
    else //Dim is positive
    {
        Allocate(d, 0);
        for (int i = 0; i < d; i++)
        {
            if (!in)
                throw std::logic_error("There is not enough inputs to match the dimensions.");

            in >> this->Point[i];
        }
    }
}
MathVector::MathVector(const MathVector& Obj) noexcept
{
    if (Obj.d == 0 || !Obj.Point)
    {
        DeAllocate();
        return;
    }

    Allocate(Obj.d, 0);

    for (unsigned int i = 0; i < d; i++)
        Point[i] = Obj.Point[i];
}
MathVector::MathVector(MathVector&& Obj) noexcept
{
    if (Obj.d == 0 || !Obj.Point)
    {
        DeAllocate();
        return;
    }

    Allocate(Obj.d, 0);

    for (unsigned int i = 0; i < d; i++)
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

    if (Obj.d == 0 || !Obj.Point)
    {
        DeAllocate();
        return *this;
    }

    if (Obj.d != d)
        Allocate(Obj.d, 0);

    for (unsigned int i = 0; i < d; i++)
        Point[i] = Obj.Point[i];

    return *this;
}
MathVector& MathVector::operator=(MathVector&& Obj) noexcept
{
    if (*this == Obj)
        return *this;

    if (Obj.d == 0 || !Obj.Point)
    {
        DeAllocate();
        return *this;
    }

    if (Obj.d != d)
        Allocate(Obj.d, 0);

    for (unsigned int i = 0; i < d; i++)
        Point[i] = Obj.Point[i];

    Obj.DeAllocate();
    return *this;
}

void MathVector::Allocate(unsigned int Dim, double Val)
{
    DeAllocate();

    Point = new double[Dim] {Val};
    d = Dim;
}
void MathVector::DeAllocate()
{
    if (Point)
    {
        delete[] Point;
        Point = nullptr;
        d = 0;
    }
}

MathVector MathVector::ErrorVector()
{
    MathVector Return(1, 0);
    Return.DeAllocate();

    return Return;
}

double& MathVector::operator[](unsigned int Index)
{
    if (Index >= d)
        throw std::logic_error("The index provided is invalid.");

    return Point[Index];
}
double MathVector::operator[](unsigned int Index) const
{
    if (Index >= d)
        throw std::logic_error("The index provided is invalid.");

    return Point[Index];
}
double MathVector::Magnitude() const
{
    if (d == 1)
        return Point[0];

    if (d <= 0)
        throw std::logic_error("A magnitude cannot be measured on a dimensionless object.");

    double Sum = 0;
    for (unsigned int i = 0; i < d; i++)
        Sum += Point[i] * Point[i];

    return sqrt(Sum);
}
double MathVector::Angle() const
{
    if (d <= 1)
        throw std::logic_error("Cannot measure the angle of a scalar or lower rank mathematical object.");

    return atan(Magnitude());
}

void MathVector::Sterilize(std::ostream& out) const noexcept
{

}
std::ostream& MathVector::operator<<(std::ostream& out) const noexcept
{

}


MathVector MathVector::CrossProduct(const MathVector& One, const MathVector& Two)
{
    if (Two.Dim() != One.Dim())
        return ErrorVector();

    MathVector A, B;
    switch (One.Dim())
    {
    case 2:
        A = MathVector(One[0], One[1], 0);
        B = MathVector(Two[0], Two[1], 0);
        break;
    case 3:
        A = One;
        B = Two;
        break;
    default:
        return ErrorVector();
    }

    return MathVector((A[1] * B[2]) - (A[2] * B[1]), (A[2] * B[0]) - (A[0] * B[2]), (A[0] * B[1]) - (A[1] * B[0])); //Uses the cross product equation.
}
double MathVector::DotProduct(const MathVector& One, const MathVector& Two)
{
    if (One.d != Two.d)
        throw std::logic_error("The dimensions of the two vectors do not match.");

    double Return = 0.0;
    for (unsigned int i = 0; i < One.d; i++)
        Return += One[i] * Two[i];

    return Return;
}

VariableType* MathVector::operator+(const VariableType& in) const noexcept
{
    try
    {
        const auto& obj = dynamic_cast<const MathVector&>(in);

        if (d != obj.d)
            return nullptr;

        auto* result = new MathVector(d);
        for (unsigned i = 0; i < d; i++)
            result->Point[i] = Point[i] + obj.Point[i];

        return result;
    }
    catch (std::bad_cast& e)
    {
        return nullptr;
    }
}
VariableType* MathVector::operator-(const VariableType& in) const noexcept
{
    try
    {
        const auto& obj = dynamic_cast<const MathVector&>(in);

        if (d != obj.d)
            return nullptr;

        auto* result = new MathVector(d);
        for (unsigned i = 0; i < d; i++)
            result->Point[i] = Point[i] - obj.Point[i];

        return result;
    }
    catch (std::bad_cast& e)
    {
        return nullptr;
    }
}
VariableType* MathVector::operator*(const VariableType& in) const noexcept
{
    try
    {
        const auto& obj = dynamic_cast<const Scalar&>(in);

        auto* result = new MathVector(*this);
        for (unsigned i = 0; i < result->d; i++)
            result->Point[i] *= obj.Data;
    }
    catch (std::bad_cast& e)
    {
        return nullptr;
    }
}
VariableType* MathVector::operator/(const VariableType& in) const noexcept
{
    try
    {
        const auto& obj = dynamic_cast<const Scalar&>(in);

        auto* result = new MathVector(*this);
        for (unsigned i = 0; i < result->d; i++)
            result->Point[i] /= obj.Data;
    }
    catch (std::bad_cast& e)
    {
        return nullptr;
    }
}

bool MathVector::operator==(const VariableType& in) const noexcept
{
    try
    {
        const auto& obj = dynamic_cast<const MathVector&>(in);

        if (d != obj.d)
            return false;

        for (unsigned i = 0; i < d; i++)
            if (Point[i] != obj.Point[i])
                return false;

        return true;
    }
    catch (std::bad_cast& e)
    {
        return false;
    }
}
bool MathVector::operator!=(const VariableType& in) const noexcept
{
    return !(*this == in);
}

MathVector::operator Matrix() const noexcept
{
    Matrix Return(d, 1);

    for (unsigned int i = 0; i < d; i++)
        Return[i][0] = Point[i];

    return Return;
}
#include "MathVector.h"

#include <utility>

MathVector::MathVector() : d(0), Point(nullptr)
{
    DeAllocate();
}
[[maybe_unused]] MathVector::MathVector(unsigned int Dim, double Val) : MathVector()
{
    if (Dim <= 0)
        throw std::logic_error("The dimension is equal to zero, and this is not allowed.");

    Allocate(Dim, Val);

    for (unsigned int i = 0; i < Dim; i++)
        Point[i] = Val;
}
MathVector::MathVector(std::istream &in) : MathVector()
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
MathVector::MathVector(const MathVector& Obj) noexcept : MathVector()
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
[[maybe_unused]] MathVector::MathVector(MathVector&& Obj) noexcept : MathVector()
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
    if (this->Point == Obj.Point) //Self assignment
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
    return {};
}

[[nodiscard]] VariableType* MathVector::MoveIntoPointer() noexcept
{
    auto* Return = new MathVector();
    Return->Point = std::exchange(this->Point, nullptr);
    Return->d = std::exchange(this->d, 0);

    DeAllocate();

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
[[maybe_unused]] [[nodiscard]] double MathVector::Magnitude() const
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
[[maybe_unused]] [[nodiscard]] double MathVector::Angle() const
{
    if (d <= 1)
        throw std::logic_error("Cannot measure the angle of a scalar or lower rank mathematical object.");

    return atan(Magnitude());
}

[[maybe_unused]] [[nodiscard]] MathVector* MathVector::FromSterilized(const std::string &obj)
{
    std::stringstream ss(obj);
    return new MathVector(ss);
}
void MathVector::Sterilize(std::ostream& out) const noexcept
{
    out << "VEC " << this->d;
    for (unsigned i = 0; i < this->d; i++)
        out << this->Point[i] << ' ';
}
[[nodiscard]] VariableTypes MathVector::GetType() const noexcept
{
    return VariableTypes::VT_Vector;
}
[[nodiscard]] std::string MathVector::GetTypeString() const noexcept
{
    std::stringstream ss;
    ss << "(Vector:";
    if (!this->IsValid())
        ss << "Err)";
    else
        ss << this->d;
    ss << ")";

    return ss.str();
}
void MathVector::Print(std::ostream& out) const noexcept
{
    out << "{ ";
    for (unsigned i = 0; i < d; i++)
        out << this->Point[i] << (i == d - 1 ? " " : ", ");
    out << '}';
}

[[maybe_unused]] [[nodiscard]] MathVector MathVector::CrossProduct(const MathVector& One, const MathVector& Two)
{
    if (Two.Dim() != One.Dim())
        throw OperatorException('X', One.GetTypeString(), Two.GetTypeString(), "Cannot cross vectors with different dimensions.");

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
        throw OperatorException('X', One.GetTypeString(), Two.GetTypeString(), "Cannot cross with these dimensions.");
    }

    return MathVector((A[1] * B[2]) - (A[2] * B[1]), (A[2] * B[0]) - (A[0] * B[2]), (A[0] * B[1]) - (A[1] * B[0])); //Uses the cross product equation.
}
[[maybe_unused]] [[nodiscard]] double MathVector::DotProduct(const MathVector& One, const MathVector& Two)
{
    if (One.d != Two.d)
        throw std::logic_error("The dimensions of the two vectors do not match.");

    double Return = 0.0;
    for (unsigned int i = 0; i < One.d; i++)
        Return += One[i] * Two[i];

    return Return;
}

MathVector MathVector::operator+(const MathVector& in) const
{
    MathVector result(*this);
    result += in;
    return result;
}
MathVector MathVector::operator-(const MathVector& in) const
{
    MathVector result(*this);
    result -= in;
    return result;
}

MathVector& MathVector::operator+=(const MathVector& in)
{
    if (!this->IsValid() || !in.IsValid())
        throw OperatorException('+', this->GetTypeString(), in.GetTypeString(), "Cannot combine error vectors.");

    if (this->d != in.d)
        throw OperatorException('+', this->GetTypeString(), in.GetTypeString(), "Dimension mismatch");

    for (unsigned i = 0; i < this->d && this->Point; i++)
        this->Point[i] += in.Point[i];

    return *this;
}
MathVector& MathVector::operator-=(const MathVector& in)
{
    if (!this->IsValid() || !in.IsValid())
        throw OperatorException('+', this->GetTypeString(), in.GetTypeString(), "Cannot combine error vectors.");

    if (this->d != in.d)
        throw OperatorException('+', this->GetTypeString(), in.GetTypeString(), "Dimension mismatch");

    for (unsigned i = 0; i < this->d && this->Point; i++)
        this->Point[i] -= in.Point[i];

    return *this;
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
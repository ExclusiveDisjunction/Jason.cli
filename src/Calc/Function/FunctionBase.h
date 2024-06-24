#pragma once

#include "../StdCalc.h"
#include "../Numerics/MathVector.h"
#include "../Numerics/Matrix.h"

class MATH_LIB FunctionBase
{
private:
    FunctionBase* Parent = nullptr;
    FunctionBase* First = nullptr, *Last = nullptr;
    FunctionBase* Next = nullptr, *Previous = nullptr;
    unsigned Children = 0;

protected:
    FunctionBase(unsigned int InputDim, unsigned int OutputDim);

    virtual void ChildRemoved(FunctionBase* Child) noexcept = 0;

    [[nodiscard]] bool PushChild(FunctionBase* New) noexcept;
    [[nodiscard]] bool PopChild(FunctionBase* obj, bool Delete = true) noexcept;

public:
    FunctionBase(const FunctionBase& Obj) = delete;
    FunctionBase(FunctionBase&& Obj) = delete;
    virtual ~FunctionBase();

    FunctionBase& operator=(const FunctionBase& Obj) = delete;
    FunctionBase& operator=(FunctionBase&& Obj) = delete;

    double A = 1.0;
    const unsigned InputDim;
    const unsigned OutputDim;

    [[nodiscard]] bool RemoveParent() noexcept;
    void ClearChildren() noexcept;
    [[nodiscard]] [[maybe_unused]] unsigned ChildCount() const noexcept { return Children; }

    [[nodiscard]] virtual MathVector Evaluate(const MathVector& X, bool& Exists) const noexcept = 0;

    [[nodiscard]] virtual bool ComparesTo(const FunctionBase* Obj) const noexcept = 0;
    [[nodiscard]] virtual bool EquatesTo(const FunctionBase* Obj) const noexcept = 0;
    [[nodiscard]] [[maybe_unused]] virtual FunctionBase* Clone() const noexcept = 0;

    virtual FunctionBase& operator-();
};
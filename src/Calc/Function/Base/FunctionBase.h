#pragma once

#include "FunctionRelation.h"
#include "../../StdCalc.h"
#include "../../MathVector.h"
#include "../../Matrix.h"

class MATH_LIB FunctionBase
{
private:
    FunctionBase* Parent = nullptr;
    FunctionBase* First = nullptr, *Last = nullptr;
    FunctionBase* Next = nullptr, *Previous = nullptr;
    unsigned Children;

protected:
    FunctionBase(unsigned int InputDim, unsigned int OutputDim);

public:
    FunctionBase(const FunctionBase& Obj) = delete;
    FunctionBase(FunctionBase&& Obj) = delete;

    FunctionBase& operator=(const FunctionBase& Obj) = delete;
    FunctionBase& operator=(FunctionBase&& Obj) = delete;

    double A = 1.0;
    const unsigned InputDim;
    const unsigned OutputDim;

    virtual bool CanHaveChildren() const noexcept;
    virtual unsigned AllowedChildrenCount() const noexcept;
    virtual bool AllowsChildAppend() const noexcept;
    void RemoveParent() noexcept;

    virtual void AddChild(FunctionBase* Child) noexcept;
    virtual void RemoveChild(FunctionBase* Child, bool Delete = true) noexcept;
    [[nodiscard]] unsigned ChildCount() const noexcept { return Children; }

    [[nodiscard]] virtual MathVector Evaluate(const MathVector& X, bool& Exists) const = 0;

    [[nodiscard]] virtual bool ComparesTo(const FunctionBase* Obj) const;
    [[nodiscard]] virtual bool EquatesTo(const FunctionBase* Obj) const;
    [[nodiscard]] [[maybe_unused]] virtual FunctionBase* Clone() const = 0;

    virtual FunctionBase& operator-();
};
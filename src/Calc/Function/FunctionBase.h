#pragma once

#include "../StdCalc.h"
#include "../MathVector.h"
#include "../Matrix.h"

class MATH_LIB FunctionBase
{
private:
    FunctionBase* Parent = nullptr;
    FunctionBase* First = nullptr, *Last = nullptr;
    FunctionBase* Next = nullptr, *Previous = nullptr;
    unsigned Children = 0;

    [[nodiscard]] bool PushChild(FunctionBase* New) noexcept;
    [[nodiscard]] bool PopChild(FunctionBase* obj) noexcept;

protected:
    FunctionBase(unsigned int InputDim, unsigned int OutputDim);

    virtual void ChildRemoved(FunctionBase* Child) = 0;

public:
    FunctionBase(const FunctionBase& Obj) = delete;
    FunctionBase(FunctionBase&& Obj) = delete;
    virtual ~FunctionBase();

    FunctionBase& operator=(const FunctionBase& Obj) = delete;
    FunctionBase& operator=(FunctionBase&& Obj) = delete;

    double A = 1.0;
    const unsigned InputDim;
    const unsigned OutputDim;

    [[nodiscard]] virtual bool CanHaveChildren() const noexcept = 0;
    [[nodiscard]] virtual unsigned AllowedChildCount() const noexcept = 0;
    [[nodiscard]] virtual bool AllowsChildAppend() const noexcept = 0;
    [[nodiscard]] bool RemoveParent() noexcept;

    [[nodiscard]] bool AddChild(FunctionBase* Child) noexcept;
    [[nodiscard]] bool RemoveChild(FunctionBase* Child, bool Delete = true) noexcept;
    void ClearChildren() noexcept;
    [[nodiscard]] unsigned ChildCount() const noexcept { return Children; }
    [[nodiscard]] bool IsFull() const { return Children >= AllowedChildCount(); }

    [[nodiscard]] virtual MathVector Evaluate(const MathVector& X, bool& Exists) const = 0;

    [[nodiscard]] virtual bool ComparesTo(const FunctionBase* Obj) const;
    [[nodiscard]] virtual bool EquatesTo(const FunctionBase* Obj) const;
    [[nodiscard]] [[maybe_unused]] virtual FunctionBase* Clone() const = 0;

    virtual FunctionBase& operator-();
};
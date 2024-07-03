/*
 * Created by exdisj
 */

#ifndef JASON_VECTORFUNCTION_H
#define JASON_VECTORFUNCTION_H

#include "../FunctionBase.h"

class MATH_LIB VectorFunction : public FunctionBase
{
private:
    void Fill(unsigned int OutputDim);
    void Clear();

    FunctionBase** Func = nullptr;
protected:
    void ChildRemoved(FunctionBase* Item) noexcept override;

public:
    VectorFunction(unsigned int InputDim, unsigned int OutputDim) ;
    template<typename Iter> requires std::forward_iterator<Iter>
    [[maybe_unused]] VectorFunction(unsigned int InputDim, unsigned int OutputDim, Iter beg, Iter end);
    template<std::convertible_to<FunctionBase*>... Args>
    [[maybe_unused]] VectorFunction(unsigned int InputDim, unsigned int OutputDim, Args... Value);
    VectorFunction(const VectorFunction& Obj) = delete;
    VectorFunction(VectorFunction&& Obj) = delete;

    VectorFunction& operator=(const VectorFunction& Obj) = delete;
    VectorFunction& operator=(VectorFunction&& Obj) = delete;

    [[maybe_unused]] void AssignFunction(unsigned int Index, FunctionBase* Func);
    [[nodiscard]] [[maybe_unused]] const FunctionBase& operator[](unsigned i) const;
    [[nodiscard]] [[maybe_unused]] FunctionBase& operator[](unsigned i);

    MathVector Evaluate(const MathVector& In, bool& Exists) const noexcept override;

    [[nodiscard]] bool ComparesTo(const FunctionBase* Obj) const noexcept override;
    [[nodiscard]] bool EquatesTo(const FunctionBase* Obj) const noexcept override;
    [[nodiscard]] FunctionBase* Clone() const noexcept override;
};

#include "VectorFunctionT.tpp"

#endif // JASON_VECTORFUNCTION_H
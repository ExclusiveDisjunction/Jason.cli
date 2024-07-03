/*
 * Created by exdisj
 */

#ifndef JASON_RATIONALFUNCTION_H
#define JASON_RATIONALFUNCTION_H

#include "../FunctionBase.h"

#include <iostream>

class MATH_LIB RationalFunction : public FunctionBase
{
private:
    void ChildRemoved(FunctionBase* Child) noexcept override { }

public:
    explicit RationalFunction(unsigned int InputDim);
    template<typename Iter> requires std::forward_iterator<Iter>
    [[maybe_unused]] RationalFunction(unsigned Dim, Iter beg, Iter end);
    template<std::convertible_to<FunctionBase*>... Args>
    [[maybe_unused]] explicit RationalFunction(unsigned Dim, Args... Objs);

    const FunctionBase& operator[](unsigned i) const;
    FunctionBase& operator[](unsigned i);

    void MultiplyFunction(FunctionBase* Obj);
    template<std::convertible_to<FunctionBase*>... Args>
    void MultiplyFunctions(Args... Obj);
    void DivideFunction(FunctionBase* Obj);

    [[maybe_unused]] [[nodiscard]] bool RemoveFunction(FunctionBase* Obj, bool Delete);

    MathVector Evaluate(const MathVector& X, bool& Exists) const noexcept override;

    [[nodiscard]] bool EquatesTo(const FunctionBase* Obj) const noexcept override;
    [[nodiscard]] bool ComparesTo(const FunctionBase* Obj) const noexcept override;
    [[nodiscard]] FunctionBase* Clone() const noexcept override;
};

#include "RationalFunctionT.tpp"

#endif //JASON_RATIONALFUNCTION_H
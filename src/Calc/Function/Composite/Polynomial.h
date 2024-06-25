/*
 * Created by exdisj
 */

#ifndef JASON_POLYNOMIAL_H
#define JASON_POLYNOMIAL_H

#include "../FunctionBase.h"

#include <iostream>

class MATH_LIB Polynomial : public FunctionBase
{
private:
    void ChildRemoved(FunctionBase* Child) noexcept override {}

public:
    Polynomial(unsigned int InputDim, unsigned int OutputDim);
    template<typename Iter> requires std::forward_iterator<Iter>
    [[maybe_unused]] Polynomial(unsigned int InputDim, unsigned int OutputDim, Iter beg, Iter end);
    template<std::convertible_to<FunctionBase*>... Args>
    [[maybe_unused]] Polynomial(unsigned int InputDim, unsigned int OutputDim, Args... Objs);
    Polynomial(const Polynomial& Obj) = delete;
    Polynomial(Polynomial&& Obj) = delete;

    Polynomial& operator=(const Polynomial& Obj) = delete;
    Polynomial& operator=(Polynomial&& Obj) = delete;

    const FunctionBase& operator[](unsigned i) const;
    FunctionBase& operator[](unsigned i);

    [[maybe_unused]] void AddFunction(FunctionBase* Obj);
    template<std::convertible_to<FunctionBase*>... Args>
    [[maybe_unused]] void AddFunctions(Args... Obj);
    [[maybe_unused]] void SubtractFunction(FunctionBase* Obj);
    template<std::convertible_to<FunctionBase*>... Args>
    [[maybe_unused]] void SubtractFunctions(Args... Obj);

    [[maybe_unused]] [[nodiscard]] bool RemoveFunction(FunctionBase* Obj, bool Delete);

    MathVector Evaluate(const MathVector& Obj, bool& Exists) const noexcept override;

    [[nodiscard]] bool EquatesTo(const FunctionBase* Obj) const noexcept override;
    [[nodiscard]] bool ComparesTo(const FunctionBase* Obj) const noexcept override;
    [[nodiscard]] FunctionBase* Clone() const noexcept override;
};

#include "PolynomialT.tpp"

#endif //JASON_POLYNOMIAL_H
#include "Polynomial.h"

template<typename Iter> requires std::forward_iterator<Iter>
[[maybe_unused]] Polynomial::Polynomial(unsigned InputDim, unsigned OutputDim, Iter beg, Iter end) : Polynomial(InputDim, OutputDim)
{
    for (Iter curr = beg; curr != end; curr++)
        AddFunction(*curr);
}
template<std::convertible_to<FunctionBase*>... Args>
[[maybe_unused]] Polynomial::Polynomial(unsigned InputDim, unsigned OutputDim, Args... Objs) : Polynomial(InputDim, OutputDim)
{
    AddFunctions(Objs...);
}

template<std::convertible_to<FunctionBase*>... Args>
[[maybe_unused]] void Polynomial::AddFunctions(Args... Obj)
{
    auto items = std::vector<FunctionBase*>({ (static_cast<FunctionBase*>(Obj))... });

    for (const auto& item : items)
        AddFunction(item);
}

template<std::convertible_to<FunctionBase*>... Args>
[[maybe_unused]] void Polynomial::SubtractFunctions(Args... Obj)
{
    auto items = std::vector<FunctionBase*>({ (static_cast<FunctionBase*>(Obj))... });

    for (const auto& item : items)
        SubtractFunction(item);
}
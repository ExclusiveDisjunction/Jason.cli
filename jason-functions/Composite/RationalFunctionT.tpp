#include "RationalFunction.h"

template<typename Iter> requires std::forward_iterator<Iter>
[[maybe_unused]] RationalFunction::RationalFunction(unsigned Dim, Iter beg, Iter end) : RationalFunction(Dim)
{
    for (Iter curr = beg; curr != end; curr++)
        MultiplyFunction(*curr);
}
template<std::convertible_to<FunctionBase*>... Args>
[[maybe_unused]] RationalFunction::RationalFunction(unsigned Dim, Args... Objs)
{
    MultiplyFunctions(Objs...);
}

template<std::convertible_to<FunctionBase*>... Args>
void RationalFunction::MultiplyFunctions(Args... Obj)
{
    auto items = std::vector<FunctionBase*>( {static_cast<FunctionBase*>(Obj)... });

    for (const auto& item : items)
        MultiplyFunction(item);
}
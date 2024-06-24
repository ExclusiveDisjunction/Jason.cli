#include "RationalFunction.h"

template<typename Iter> requires std::forward_iterator<Iter>
[[maybe_unused]] RationalFunction::RationalFunction(unsigned Dim, Iter beg, Iter end)
{

}
template<std::convertible_to<FunctionBase*>... Args>
[[maybe_unused]] RationalFunction::RationalFunction(unsigned Dim, Args... Objs)
{

}

template<std::convertible_to<FunctionBase*>... Args>
void RationalFunction::MultiplyFunctions(Args... Obj)
{

}
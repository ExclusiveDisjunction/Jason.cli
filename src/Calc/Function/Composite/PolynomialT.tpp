#include "Polynomial.h"

template<typename Iter> requires std::forward_iterator<Iter>
[[maybe_unused]] Polynomial::Polynomial(unsigned InputDim, unsigned OutputDim, Iter beg, Iter end)
{

}
template<std::convertible_to<FunctionBase*>... Args>
[[maybe_unused]] Polynomial::Polynomial(unsigned InputDim, unsigned OutputDim, Args... Objs)
{

}

template<std::convertible_to<FunctionBase*>... Args>
[[maybe_unused]] void Polynomial::AddFunctions(Args... Obj)
{

}

template<std::convertible_to<FunctionBase*>... Args>
[[maybe_unused]] void Polynomial::SubtractFunctions(Args... Obj)
{

}
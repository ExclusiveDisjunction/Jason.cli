#include "VectorFunction.h"

template<typename Iter> requires std::forward_iterator<Iter>
[[maybe_unused]] VectorFunction::VectorFunction(unsigned InputDim, unsigned OutputDim, Iter beg, Iter end) : VectorFunction(InputDim, OutputDim)
{
    unsigned runs = 0;
    for (Iter curr = beg; curr != end && runs < OutputDim; curr++, runs++)
        this->Func[runs] = *curr;

    if (runs != OutputDim - 1) //Not enough filled
        throw std::logic_error("Not enough functions were supplied to match the OutputDim parameter.");
}
template<std::convertible_to<FunctionBase*>... Args>
[[maybe_unused]] VectorFunction::VectorFunction(unsigned int InputDim, unsigned int OutputDim, Args... Value) : VectorFunction(InputDim, OutputDim)
{
    std::vector<FunctionBase*> ToFill = std::vector<FunctionBase*>({ (static_cast<FunctionBase*>(Value))... });
    if (ToFill.size() != OutputDim)
        return;

    for (unsigned int i = 0; i < OutputDim; i++)
    {
        if (!this->PushChild(ToFill[i]))
            throw std::logic_error("Empty item, or invalid function dimensions.");

        this->Func[i] = ToFill[i];
    }
}
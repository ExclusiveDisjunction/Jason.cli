//
// Created by exdisj on 6/24/24.
//

#ifndef JASON_FUNCTIONITERATOR_H
#define JASON_FUNCTIONITERATOR_H

#include "../StdCalc.h"

class MATH_LIB FunctionBase;

template<typename ReferenceType, typename PointerType>
class MATH_LIB FunctionIteratorBase
{
private:
    FunctionBase* Curr = nullptr;

public:
    explicit FunctionIteratorBase(FunctionBase* Input);

    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = long long;
    using value_type = ReferenceType;
    using pointer = PointerType;
    using reference = ReferenceType&;

    reference operator*() const;
    pointer operator->();

    FunctionIteratorBase& operator++();
    FunctionIteratorBase operator++(int);
    FunctionIteratorBase& operator--();
    FunctionIteratorBase operator--(int);

    bool operator==(const FunctionIteratorBase& b) const;
    bool operator!=(const FunctionIteratorBase& b) const;
};

using FunctionIterator = FunctionIteratorBase<FunctionBase&, FunctionBase*>;
using ConstFunctionIterator = FunctionIteratorBase<const FunctionBase&, const FunctionBase*>;

#endif //JASON_FUNCTIONITERATOR_H

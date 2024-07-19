//
// Created by exdisj on 6/24/24.
//

#ifndef JASON_FUNCTIONITERATOR_H
#define JASON_FUNCTIONITERATOR_H

#include <iterator>
#include "../Calc/StdCalc.h"

class MATH_LIB FunctionBase;

class MATH_LIB FunctionIterator
{
private:
    FunctionBase* Curr = nullptr;

public:
    explicit FunctionIterator(FunctionBase* Input);

    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = long long;
    using value_type = FunctionBase&;
    using pointer = FunctionBase*;
    using reference = value_type&;

    reference operator*() const;
    pointer operator->();

    FunctionIterator& operator++();
    FunctionIterator operator++(int);
    FunctionIterator& operator--();
    FunctionIterator operator--(int);

    bool operator==(const FunctionIterator& b) const;
    bool operator!=(const FunctionIterator& b) const;
};

class MATH_LIB ConstFunctionIterator
{
private:
    FunctionBase* Curr = nullptr;

public:
    explicit ConstFunctionIterator(FunctionBase* Input);

    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = long long;
    using value_type = const FunctionBase&;
    using pointer = const FunctionBase*;
    using reference = value_type&;

    reference operator*() const;
    pointer operator->();

    ConstFunctionIterator& operator++();
    ConstFunctionIterator operator++(int);
    ConstFunctionIterator& operator--();
    ConstFunctionIterator operator--(int);

    bool operator==(const ConstFunctionIterator& b) const;
    bool operator!=(const ConstFunctionIterator& b) const;
};

#endif //JASON_FUNCTIONITERATOR_H

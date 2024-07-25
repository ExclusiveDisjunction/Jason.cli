//
// Created by exdisj on 6/24/24.
//

#include "FunctionIterator.h"
#include "FunctionBase.h"
#include <iostream>

FunctionIterator::FunctionIterator(FunctionBase* Input) : Curr(Input)
{

}

FunctionIterator::reference FunctionIterator::operator*() const
{
    if (!Curr)
        throw std::logic_error("Null dereference");

    return const_cast<reference>(*Curr);
}
FunctionIterator::pointer FunctionIterator::operator->()
{
    if (!Curr)
        throw std::logic_error("Null dereference");

    return const_cast<pointer>(Curr);
}

FunctionIterator& FunctionIterator::operator++()
{
    if (Curr)
        Curr = Curr->Next;

    return *this;
}
FunctionIterator FunctionIterator::operator++(int)
{
    FunctionIterator result(*this);
    this->operator++();

    return result;
}
FunctionIterator& FunctionIterator::operator--()
{
    if (Curr)
        Curr = Curr->Previous;

    return *this;
}
FunctionIterator FunctionIterator::operator--(int)
{
    FunctionIterator result(*this);
    this->operator--();

    return result;
}

bool FunctionIterator::operator==(const FunctionIterator& b) const
{
    return this->Curr == b.Curr;
}
bool FunctionIterator::operator!=(const FunctionIterator& b) const
{
    return this->Curr != b.Curr;
}

ConstFunctionIterator::ConstFunctionIterator(FunctionBase* Input) : Curr(Input)
{

}

ConstFunctionIterator::reference ConstFunctionIterator::operator*() const
{
    if (!Curr)
        throw std::logic_error("Null dereference");

    return const_cast<const FunctionBase&>(*Curr);
}
ConstFunctionIterator::pointer ConstFunctionIterator::operator->()
{
    if (!Curr)
        throw std::logic_error("Null dereference");

    return const_cast<const FunctionBase*>(Curr);
}

ConstFunctionIterator& ConstFunctionIterator::operator++()
{
    if (Curr)
        Curr = Curr->Next;

    return *this;
}
ConstFunctionIterator ConstFunctionIterator::operator++(int)
{
    ConstFunctionIterator result(*this);
    this->operator++();

    return result;
}
ConstFunctionIterator& ConstFunctionIterator::operator--()
{
    if (Curr)
        Curr = Curr->Previous;

    return *this;
}
ConstFunctionIterator ConstFunctionIterator::operator--(int)
{
    ConstFunctionIterator result(*this);
    this->operator--();

    return result;
}

bool ConstFunctionIterator::operator==(const ConstFunctionIterator& b) const
{
    return this->Curr == b.Curr;
}
bool ConstFunctionIterator::operator!=(const ConstFunctionIterator& b) const
{
    return this->Curr != b.Curr;
}
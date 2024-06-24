#include "FunctionBase.h"

FunctionBase::FunctionBase(unsigned int Input, unsigned int Output) : InputDim(Input), OutputDim(Output)
{
    if (Input == 0 || Output == 0)
        throw std::logic_error("Cannot construct a function with an input or output of zero.");
}
FunctionBase::~FunctionBase()
{
    ClearChildren();
    (void)RemoveParent();
}

[[nodiscard]] bool FunctionBase::RemoveParent() noexcept
{
    if (!Parent)
        return true;

    if (!Parent->PopChild(this, false))
        return false;

    this->Parent = nullptr;
    return true;
}

[[nodiscard]] bool FunctionBase::PushChild(FunctionBase* New) noexcept
{
    if (!New || New->Parent == this || IsFull()) //Empty, already contained, or full.
        return false;

    if (New->Parent != nullptr && !New->RemoveParent())
        return false;

    New->Parent = this;

    if (!First || !Last) //Empty
    {
        First = Last = New;
        New->Next = New->Previous = nullptr;
    }
    else if (First == Last) //One element
    {
        First->Next = Last = New;
        New->Previous = First;
        New->Next = nullptr;
    }
    else
    {
        New->Previous = Last->Previous;
        New->Next = nullptr;
        Last->Next = Last = New;
    }

    Children++;
    return true;
}
[[nodiscard]] bool FunctionBase::PopChild(FunctionBase* obj, bool Delete) noexcept
{
    if (!obj || obj->Parent != this) //Null or not contained
        return false;

    if (!First || !Last) //Empty list?
        return false;
    else if (First == Last && First == obj) //One item
        First = Last = nullptr;
    else if (First == obj) //The first item in this list
    {
        First = First->Next;
        if (First)
            First->Previous = nullptr;
    }
    else if (Last == obj) //last item in this list
    {
        Last = Last->Previous;
        if (Last)
            Last->Next = nullptr;
    }
    else //We have to rejoin the list after removing it.
    {
        auto* p = obj->Previous, *n = obj->Next;
        if (p)
            p->Next = n;
        if (n)
            n->Previous = p;
    }

    Children--;
    obj->Next = obj->Previous = obj->Parent = nullptr;
    if (Delete)
    {
        delete obj;
        obj = nullptr;
    }
    return true;
}
void FunctionBase::ClearChildren() noexcept
{
    if (!First && !Last)
        return;
    else if (First == Last) //One element
        delete First;
    else
    {
        FunctionBase* curr = First;
        while (curr)
        {
            FunctionBase* tmp = curr->Next;
            delete curr;
            curr = tmp;
        }
    }

    First = Last = nullptr;
    Children = 0;
}

FunctionBase& FunctionBase::operator-()
{
    A = -A; return *this;
}
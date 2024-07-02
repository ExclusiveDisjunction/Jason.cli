#include "FunctionBase.h"
#include <utility>

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
    if (!New || New->Parent == this || New->InputDim != this->InputDim || New->OutputDim != this->OutputDim) //Empty, already contained, or full.
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
    else
        Flags = 0; //Clears all flags from the function, so they can be re-used without problems.

    return true;
}
void FunctionBase::PushAndBind(FunctionBase*& BindTo, FunctionBase* Child)
{
    if (BindTo == Child)
        return;

    delete BindTo;
    BindTo = nullptr;

    if (!this->PushChild(Child))
        throw std::logic_error("Could not append this child.");

    BindTo = Child;
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

void FunctionBase::CloneChildrenFrom(const FunctionBase* Obj, bool ClearCurr)
{
    if (!Obj)
        return;

    if (ClearCurr)
        this->ClearChildren();

    auto end = Obj->LastChild();
    for (auto curr = Obj->FirstChild(); curr != end; curr++)
    {
        if (!PushChild(curr->Clone()))
            throw std::logic_error("Could not one of the children functions.");
    }
}
void FunctionBase::StealChildrenFrom(FunctionBase* Obj, bool ClearCurr) noexcept
{
    if (!Obj)
        return;

    if (ClearCurr)
        this->ClearChildren();

    if (ClearCurr) //Optimized, easy case
    {
        this->First = std::exchange(Obj->First, nullptr);
        this->Last = std::exchange(Obj->Last, nullptr);
        this->Children = std::exchange(Obj->Children, 0);

        for (FunctionBase* curr = this->First; curr != nullptr; curr = curr->Next)
            curr->Parent = this;
    }
    else
    {
        //First, change ownership with children.
        for (FunctionBase* curr = Obj->First; curr != nullptr; curr = curr->Next)
            curr->Parent = this;

        this->Last->Next = std::exchange(Obj->First, nullptr);
        this->Last = std::exchange(Obj->Last, nullptr);
        this->Children += std::exchange(Obj->Children, 0);
    }
}

const FunctionBase& FunctionBase::GetChildAt(unsigned i) const
{
    if (i >= this->ChildCount())
        throw std::logic_error("Out of bounds");

    ConstFunctionIterator iter = this->FirstChild(), last = this->LastChild();
    for (int j = 0; j < i && iter != last; j++)
        iter++;

    return *iter;
}
FunctionBase& FunctionBase::GetChildAt(unsigned i)
{
    return const_cast<FunctionBase&>(const_cast<const FunctionBase*>(this)->GetChildAt(i));
}

FunctionIterator FunctionBase::FirstChild() noexcept
{
    return !First ? LastChild() : FunctionIterator(First);
}
FunctionIterator FunctionBase::LastChild() noexcept
{
    return FunctionIterator(Last);
}
ConstFunctionIterator FunctionBase::FirstChild() const noexcept
{
    return !First ? LastChild() : ConstFunctionIterator(First);
}
ConstFunctionIterator FunctionBase::LastChild() const noexcept
{
    return ConstFunctionIterator(Last);
}

[[nodiscard]] bool FunctionBase::FlagActive(FunctionFlags Flag) const noexcept
{
    return Flags & Flag;
}
void FunctionBase::SetFlag(FunctionFlags Flag, bool Active) noexcept
{
    bool Prev = FlagActive(Flag);
    if (Prev == Active)
        return;

    unsigned char NewFlag = Active ? Flag : ~Flag;
    this->Flags &= NewFlag;
}
void FunctionBase::InvertFlag(FunctionFlags Flag) noexcept
{
    bool Prev = FlagActive(Flag);
    unsigned char Result;
    if (Prev)
        Result = static_cast<unsigned char>(~Flag);
    else
        Result = static_cast<unsigned char>(Flag);

    this->Flags &= Result;
}

FunctionBase& FunctionBase::Get(FunctionBase* Binding)
{
    if (!Binding)
        throw std::logic_error("Cannot dereference an empty function.");

    return *Binding;
}

FunctionBase& FunctionBase::operator-()
{
    A = -A; return *this;
}
/*
 * Created by exdisj
 */

#ifndef JASON_FUNCTIONBASE_H
#define JASON_FUNCTIONBASE_H

#include "FunctionIterator.h"
#include "../Calc/StdCalc.h"
#include "../Calc/Numerics/MathVector.h"
#include "../Calc/Numerics/Matrix.h"

enum FunctionFlags
{
    FF_Poly_Neg = 1, //For negation, polynomials
    FF_Rat_Inv = 2, //For inversion, rational functions
};

/// \brief Represents a function, or a mathematical object with a specified number of inputs and output. This class also handles all children of a function.
class MATH_LIB FunctionBase
{
private:
    FunctionBase* Parent = nullptr;
    FunctionBase* First = nullptr, *Last = nullptr;
    FunctionBase* Next = nullptr, *Previous = nullptr;
    unsigned Children = 0;

    unsigned char Flags = 0;

protected:
    /// \brief Stores the values of Input and Output in the variables of the class. Throws if either one is zero.
    /// \param InputDim The number of input variables to the function.
    /// \param OutputDim The number of outputs this function gives.
    FunctionBase(unsigned int InputDim, unsigned int OutputDim);

    /// \brief Called when PopChild is called, and tells the current instance to 'let go' of the pointer. Do not delete 'Child'
    /// \param Child The child being removed.
    virtual void ChildRemoved(FunctionBase* Child) noexcept = 0;

    /// \breif Inserts a child into the list, and calls New->RemoveParent() if New->Parent != nullptr. Fails if The dimensions do not match.
    [[nodiscard]] bool PushChild(FunctionBase* New) noexcept;
    /// \breif Removes a child from the list, presuming that the child is contained in this list.
    /// \param obj The function to remove.
    /// \param Delete Deletes 'obj' if true.
    /// \return True if the child was contained and deleted, false if not contained.
    [[nodiscard]] bool PopChild(FunctionBase* obj, bool Delete = true) noexcept;

    void PushAndBind(FunctionBase*& BindTo, FunctionBase* Child);
    [[nodiscard]] static FunctionBase& Get(FunctionBase* Binding);

    [[nodiscard]] const FunctionBase& GetChildAt(unsigned i) const;
    [[nodiscard]] FunctionBase& GetChildAt(unsigned i);

    //Clones all children from another function, and if ClearCurr is true, clears this instance beforehand.
    void CloneChildrenFrom(const FunctionBase* Obj, bool ClearCurr);
    //Takes all children from another function, and if ClearCurr is true, clears this instance beforehand.
    void StealChildrenFrom(FunctionBase* Obj, bool ClearCurr) noexcept;

    [[nodiscard]] ConstFunctionIterator FirstChild() const noexcept;
    [[nodiscard]] ConstFunctionIterator LastChild() const noexcept;

    [[nodiscard]] FunctionIterator FirstChild() noexcept;
    [[nodiscard]] FunctionIterator LastChild() noexcept;

public:
    FunctionBase(const FunctionBase& Obj) = delete;
    FunctionBase(FunctionBase&& Obj) = delete;
    virtual ~FunctionBase();

    friend class FunctionIterator;
    friend class ConstFunctionIterator;

    FunctionBase& operator=(const FunctionBase& Obj) = delete;
    FunctionBase& operator=(FunctionBase&& Obj) = delete;

    // The 'leading constant', or the factor that is multiplied by the result of each computation.
    double A = 1.0;
    // The number if inputs
    const unsigned InputDim;
    // The number of outputs.
    const unsigned OutputDim;

    /// \breif If this->Parent != nullptr, calls this->Parent->PopChild(this).
    /// \return True if the pop was successful, or if the parent was nullptr. False otherwise.
    [[nodiscard]] bool RemoveParent() noexcept;
    /// \brief Deletes all children.
    void ClearChildren() noexcept;
    /// \breif Returns the number of children currently contained.
    [[nodiscard]] [[maybe_unused]] unsigned ChildCount() const noexcept { return Children; }

    /// \breif Takes the input 'X', and performs the operation of the function to output a result.
    /// \param X The input to the function. If X.Dim() != InputDim, then 'Exists' will be false.
    /// \param Exists When true, the function as properly evaluated, and if false, do not use the output of the function.
    /// \return MathVector::ErrorVector() if 'Exists' is false, otherwise a MathVector of dimension 'this->OutputDim' containing the result of the computation.
    [[nodiscard]] virtual MathVector Evaluate(const MathVector& X, bool& Exists) const noexcept = 0;

    [[nodiscard]] bool FlagActive(FunctionFlags Flag) const noexcept;
    void SetFlag(FunctionFlags Flag, bool Active) noexcept;
    void InvertFlag(FunctionFlags Flag) noexcept;

    /// \breif Compares two functions and determines if they are comparable.
    /// \return True if the functions are comparable, or can be added together & simplify, false otherwise.
    [[nodiscard]] virtual bool ComparesTo(const FunctionBase* Obj) const noexcept = 0;
    /// \breif Compares two functions and determines if they are equal.
    /// \return True if the functions are equal, or if you have function f and g, then if f + g = 2f, then f and g are equal. Returns false otherwise.
    [[nodiscard]] virtual bool EquatesTo(const FunctionBase* Obj) const noexcept = 0;
    /// \breif Returns a new instance of the current function with the same values.
    [[nodiscard]] [[maybe_unused]] virtual FunctionBase* Clone() const noexcept = 0;

    virtual FunctionBase& operator-();
};

#include "FunctionIterator.h"

#endif
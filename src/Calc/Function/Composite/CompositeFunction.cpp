#include "CompositeFunction.h"

namespace Math::Function
{
	CompositeFunction::CompositeFunction(unsigned int Input, unsigned int Output) : FunctionBase(Input, Output) {}

	FunctionBase* CompositeFunction::operator[](unsigned int i) const
	{
		FunctionRelationSeg* Obj = Retrive(i);
		
		return !Obj ? nullptr : Obj->Target;
	}
}
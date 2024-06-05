#include "..\CoreFunctions.h"

namespace Math::Function
{
	Constant::Constant(unsigned int InputDim, double A) : FunctionBase(InputDim, 1)
	{
		_Data[0] = A;
	}
	Constant::Constant(unsigned int InputDim, MathVector Data) : FunctionBase(InputDim, Data.Dim())
	{
		_Data = Data;
	}

	void Constant::ChildRemoved(FunctionBase* Child)
	{
		//Constants do not have children.
	}

	MathVector Constant::Evaluate(const MathVector& X, bool& Exists) const
	{
		if (X.Dim() != InputDim())
		{
			Exists = false;
			return MathVector::ErrorVector();
		}

		Exists = true;
		return A * _Data;
	}

	bool Constant::ComparesTo(FunctionBase* const& Obj) const
	{
		Constant* Conv = dynamic_cast<Constant*>(Obj);
		return Conv && Conv->_Input == _Input && Conv->_Output == _Output;
	}
	bool Constant::EquatesTo(FunctionBase* const& Obj) const
	{
		Constant* Conv = dynamic_cast<Constant*>(Obj);
		return Conv && Conv->_Input == _Input && Conv->_Output == _Output && Conv->_Data == _Data;
	}
	FunctionBase* Constant::Clone() const
	{
		return new Constant(_Input, _Data);
	}
}
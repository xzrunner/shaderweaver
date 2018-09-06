#include "sw/VariableType.h"

namespace sw
{

std::string VariableType::ToGLSL() const
{
	std::string str;

	switch (qualifier)
	{
	case VT_CONST:
		str += "const ";
		break;
	case VT_ATTRIBUTE:
		str += "attribute ";
		break;
	case VT_UNIFORM:
		str += "uniform ";
		break;
	case VT_VARYING:
		str += "varying ";
		break;
	}

	switch (interp)
	{
	case VT_SAMPLER_2D:
		str += "sampler2D";
		break;
	case VT_SAMPLER_CUBE:
		str += "samplerCube";
		break;
	case VT_MAT:
		str += "mat";
		break;
	}

	if (dim == VT_1)
	{
		switch (precision)
		{
		case VT_FLOAT:
			str += "float";
			break;
		case VT_INT:
			str += "int";
			break;
		case VT_BOOL:
			str += "bool";
			break;
		}
	}
	else
	{
		switch (precision)
		{
		case VT_INT:
			str += "i";
			break;
		case VT_BOOL:
			str += "b";
			break;
		}
		str += "vec";
		switch (dim)
		{
		case VT_2:
			str += "2";
			break;
		case VT_3:
			str += "3";
			break;
		case VT_4:
			str += "4";
			break;
		}
	}

	return str;
}

}
#include "shaderweaver/VariableType.h"
#include "shaderweaver/predef.h"

namespace sw
{

std::string VariableType::ToGLSL(ShaderType st) const
{
	std::string str;

	switch (qualifier)
	{
	case VT_CONST:
		str += "const ";
		break;
	case VT_UNIF:
		str += "uniform ";
		break;
#ifdef GLSL_OLD
	case VT_SHADER_IN:
		switch (st)
		{
		case ST_VERT:
			str += "attribute ";
			break;
		case ST_FRAG:
			str += "varying ";
			break;
		}
		break;
	case VT_SHADER_OUT:
		str += "varying ";
		break;
#else
	case VT_SHADER_IN:
		str += "in ";
		break;
	case VT_SHADER_OUT:
		str += "out ";
		break;
#endif // GLSL_OLD
	}

	switch (interp)
	{
	case VT_T2D:
		str += "sampler2D";
		break;
	case VT_TCUBE:
		str += "samplerCube";
		break;
	case VT_MAT:
		switch (dim)
		{
		case VT_2:
			str += "mat2";
			break;
		case VT_3:
			str += "mat3";
			break;
		case VT_4:
			str += "mat4";
			break;
		}
		break;
	default:
		if (dim == VT_1)
		{
			switch (precision)
			{
			case VT_FLT:
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
	}

	return str;
}

}
#pragma once

#include "sw/Node.h"
#include "sw/VariableType.h"

namespace sw
{
namespace node
{

class Tex2DSample : public sw::Node
{
public:
	Tex2DSample()
		: Node("Tex2DSample")
	{
		AddVariable(Variable(t_tex2d | t_n_in, "tex"));
		AddVariable(Variable(t_uv    | t_n_in, "uv"));

		AddVariable(Variable(t_col4 | t_read | t_n_out, "col4"));
	}

	enum ImportIdx
	{
		IN_TEX = 0,
		IN_UV,
	};

protected:
	virtual std::string GetBody() const override
	{
		// todo: in phong, result will be changed by another one before using
		// 1. clip phong to small block, or
		// 2. add special flag to make result read only
		return "(col4) = texture2D((tex), (uv));\n";
	}

}; // Tex2DSample

}
}
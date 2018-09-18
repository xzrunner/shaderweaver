#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class SampleTex2D : public sw::Node
{
public:
	SampleTex2D()
		: Node("SampleTex2D")
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

}; // SampleTex2D

}
}
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

		AddVariable(Variable(t_col4 | t_n_out, "col4"));
	}

	enum ImportIdx
	{
		IN_TEX = 0,
		IN_UV,
	};

protected:
	virtual std::string GetBody() const override
	{
		return "(col4) = texture2D((tex), (uv));\n";
	}

}; // Tex2DSample

}
}
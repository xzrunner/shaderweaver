#pragma once

#include "sw/Node.h"
#include "sw/VariableType.h"

namespace sw
{
namespace node
{

class Tex2DSample : public Node
{
public:
	Tex2DSample()
		: Node("Tex2DSample")
	{
		AddVariable(Variable(t_tex2d | t_in, "tex"));
		AddVariable(Variable(t_uv    | t_in, "uv"));

		AddVariable(Variable(t_col4 | t_out, "col4"));
	}

protected:
	virtual std::string GetBody() const override
	{
		return "(col4) = texture2D((tex), (uv));\n";
	}

}; // Tex2DSample

}
}
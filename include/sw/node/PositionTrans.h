#pragma once

#include "sw/Node.h"
#include "sw/VariableType.h"

namespace sw
{
namespace node
{

class PositionTrans : public Node
{
public:
	PositionTrans()
		: Node("PositionTrans")
	{
		AddVariable(Variable(t_vec4 | t_unif | t_in, "projection"));
		AddVariable(Variable(t_vec4 | t_unif | t_in, "modelview"));
		AddVariable(Variable(t_vec4 | t_attr | t_in, "position"));
		AddVariable(Variable(t_vec4 | t_out, "pos"));
	}

protected:
	virtual std::string GetBody() const override
	{
		return "pos = u_projection * u_modelview * position;\n";
	}

}; // PositionTrans

}
}
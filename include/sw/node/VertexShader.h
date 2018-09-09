#pragma once

#include "sw/Node.h"
#include "sw/VariableType.h"

namespace sw
{
namespace node
{

class VertexShader : public Node
{
public:
	VertexShader()
		: Node("VertexShader")
	{
		AddVariable(Variable(t_pos4 | t_n_in, "pos"));
		AddVariable(Variable(t_pos4 | t_n_out | t_s_end, "gl_Position"));
	}

protected:
	virtual std::string GetBody() const override
	{
		return "(gl_Position) = (pos);\n";
	}

}; // VertexShader

}
}
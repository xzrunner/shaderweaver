#pragma once

#include "sw/Node.h"
#include "sw/VariableType.h"

namespace sw
{
namespace node
{

class Add : public Node
{
public:
	Add()
		: Node("Add")
	{
		AddVariable(Variable(t_vec4, "a"));
		AddVariable(Variable(t_vec4 | t_in, "b"));
		AddVariable(Variable(t_vec4 | t_out, "sum"));
	}

protected:
	virtual std::string GetBody() const override
	{
		return "sum = a + b;\n";
	}

}; // Add

}
}
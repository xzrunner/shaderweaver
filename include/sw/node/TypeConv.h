#pragma once

#include "sw/Node.h"
#include "sw/VariableType.h"

namespace sw
{
namespace node
{

class TypeConv : public Node
{
public:
	TypeConv()
		: Node("TypeConv")
	{
		AddVariable(Variable(t_n_in, "from"));
		AddVariable(Variable(t_n_out, "to"));
	}

protected:
	virtual std::string GetBody() const override
	{
		return "(to) = (from);\n";
	}

}; // TypeConv

}
}
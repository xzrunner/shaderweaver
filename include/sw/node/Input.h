#pragma once

#include "sw/Node.h"
#include "sw/VariableType.h"

namespace sw
{
namespace node
{

class Input : public Node
{
public:
	Input(const std::string& name, uint32_t type)
		: Node("Input")
	{
		AddVariable(Variable(t_out | t_sd_in | type, name));
	}

protected:
	virtual std::string GetBody() const override
	{
		return "";
	}

}; // Input

}
}
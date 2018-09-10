#pragma once

#include "sw/Node.h"
#include "sw/VariableType.h"

namespace sw
{
namespace node
{

class Uniform : public sw::Node
{
public:
	Uniform(const std::string& name, uint32_t type)
		: Node("Uniform")
	{
		AddVariable(Variable(t_n_out | t_unif | type, name));
	}

protected:
	virtual std::string GetBody() const override
	{
		return "";
	}

}; // Uniform

}
}
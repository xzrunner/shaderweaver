#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

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
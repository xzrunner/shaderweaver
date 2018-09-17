#pragma once

#include "sw/Node.h"
#include "sw/VariableType.h"

namespace sw
{
namespace node
{

class UV : public Node
{
public:
	UV(const std::string& name)
		: Node("UV")
	{
		AddVariable(Variable(t_uv | t_n_out | t_s_in, name));
	}

protected:
	virtual std::string GetBody() const override
	{
		return "";
	}

}; // UV

}
}
#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

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
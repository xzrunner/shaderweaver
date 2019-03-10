#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class ShaderUniform : public sw::Node
{
public:
	ShaderUniform(const std::string& name = "", uint32_t type = 0)
		: Node("ShaderUniform")
	{
		SetNameAndType(name, type);
	}

	void SetNameAndType(const std::string& name, uint32_t type) {
		InitVariables({
		}, {
			{ t_unif | type, name },
		}, {
		});
	}

protected:
	virtual std::string GetBody() const override
	{
		return "";
	}

	RTTR_ENABLE(Node)

}; // ShaderUniform

}
}
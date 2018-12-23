#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class Input : public sw::Node
{
public:
	Input(const std::string& name = "", uint32_t type = 0)
		: Node("Input")
	{
		SetNameAndType(name, type);
	}

	void SetNameAndType(const std::string& name, uint32_t type) {
		InitVariables({
		}, {
			{ t_s_in | type, name },
		}, {
		});
	}

protected:
	virtual std::string GetBody() const override
	{
		return "";
	}

	RTTR_ENABLE(Node)

}; // Input

}
}
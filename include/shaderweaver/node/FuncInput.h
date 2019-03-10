#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class FuncInput : public sw::Node
{
public:
	FuncInput(const std::string& name = "", uint32_t type = 0)
		: Node("FuncInput")
	{
		SetNameAndType(name, type);
	}

	void SetNameAndType(const std::string& name, uint32_t type) {
		InitVariables({
		}, {
			{ t_f_in | type, name },
		}, {
		});
	}

protected:
	virtual std::string GetBody() const override
	{
		return "";
	}

	RTTR_ENABLE(Node)

}; // FuncInput

}
}
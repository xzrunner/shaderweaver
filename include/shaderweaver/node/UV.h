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
	UV(const std::string& name = "uv")
		: Node("UV")
	{
		SetName(name);
	}

	void SetName(const std::string& name)
	{
		InitVariables({
		}, {
			{ t_uv | t_s_in, name },
		}, {
		});
	}

protected:
	virtual std::string GetBody() const override
	{
		return "";
	}

	RTTR_ENABLE(Node)

}; // UV

}
}
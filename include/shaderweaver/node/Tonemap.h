#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class Tonemap : public sw::Node
{
public:
	Tonemap()
		: Node("Tonemap")
	{
		InitVariables({
			{ t_col3, "_in" },
		}, {
			{ t_col3, "_out" },
		}, {
		});
	}

protected:
	virtual std::string GetBody() const override
	{
		return R"(
#_out# = #_in# / (#_in# + vec3(1.0));
)" + 1;
	}

	RTTR_ENABLE(Node)

}; // Tonemap

}
}
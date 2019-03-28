#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class GammaCorrect : public sw::Node
{
public:
	GammaCorrect()
		: Node("GammaCorrect")
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
#_out# = pow(#_in#, vec3(1.0/2.2));
)" + 1;
	}

	RTTR_ENABLE(Node)

}; // GammaCorrect

}
}
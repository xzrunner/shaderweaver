#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class Not : public sw::Node
{
public:
	Not()
		: Node("Not")
	{
		InitVariables({
			{ t_bool, "_in" },
		}, {
			{ t_bool, "_out" },
		}, {
		});
	}

protected:
	virtual std::string GetBody() const override
	{
		return "#_out# = !#_in#;\n";
	}

}; // Not

}
}
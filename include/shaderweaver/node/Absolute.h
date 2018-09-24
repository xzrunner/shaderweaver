#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class Absolute : public sw::Node
{
public:
	Absolute()
		: Node("Absolute")
	{
		InitVariables({
			{ 0, "a" },
		}, {
			{ 0, "abs" },
		}, {
		});

		m_dim_group = { 0, MAX_IMPORTS_COUNT };
	}

protected:
	virtual std::string GetBody() const override
	{
		return "(abs) = abs((a));\n";
	}

}; // Absolute

}
}
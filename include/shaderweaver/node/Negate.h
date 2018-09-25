#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"

namespace sw
{
namespace node
{

class Negate : public sw::Node
{
public:
	Negate()
		: Node("Negate")
	{
		InitVariables({
			{ 0, "a" },
		}, {
			{ 0, "neg" },
		}, {
		});

		m_dim_group = { 0, MAX_IMPORTS_COUNT };
	}

protected:
	virtual std::string GetBody() const override
	{
		return "(neg) = -1 * (a);\n";
	}

}; // Negate

}
}
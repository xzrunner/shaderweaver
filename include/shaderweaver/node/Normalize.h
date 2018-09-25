#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"

namespace sw
{
namespace node
{

class Normalize : public sw::Node
{
public:
	Normalize()
		: Node("Normalize")
	{
		InitVariables({
			{ 0, "a" },
		}, {
			{ 0, "norm" },
		}, {
		});

		m_dim_group = { 0, MAX_IMPORTS_COUNT };
	}

protected:
	virtual std::string GetBody() const override
	{
		return "(norm) = normalize((a));\n";
	}

}; // Normalize

}
}
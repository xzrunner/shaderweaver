#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"

namespace sw
{
namespace node
{

class Arctangent : public sw::Node
{
public:
	Arctangent()
		: Node("Arctangent")
	{
		InitVariables({
			{ 0, "_in" },
		}, {
			{ 0, "_out" },
		}, {
		});

		m_dim_group = { 0, MAX_IMPORTS_COUNT };
	}

protected:
	virtual std::string GetBody() const override
	{
		return "#_out# = atan(#_in#);\n";
	}

	RTTR_ENABLE(Node)

}; // Arctangent

}
}
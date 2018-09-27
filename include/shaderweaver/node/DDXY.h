#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"

namespace sw
{
namespace node
{

class DDXY : public sw::Node
{
public:
	DDXY()
		: Node("DDXY")
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
		return "#_out# = abs(dFdx(#_in#)) + abs(dFdy(#_in#));\n";
	}

}; // DDXY

}
}
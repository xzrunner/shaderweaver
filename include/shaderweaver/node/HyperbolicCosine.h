#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"

namespace sw
{
namespace node
{

class HyperbolicCosine : public sw::Node
{
public:
	HyperbolicCosine()
		: Node("HyperbolicCosine", 130)
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
		return "#_out# = cosh(#_in#);\n";
	}

}; // HyperbolicCosine

}
}
#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"

namespace sw
{
namespace node
{

class Saturate : public sw::Node
{
public:
	Saturate()
		: Node("Saturate")
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
		return "#_out# = clamp(#_in#, 0.0, 1.0);\n";
	}

}; // Saturate

}
}
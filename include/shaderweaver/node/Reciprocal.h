#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"

namespace sw
{
namespace node
{

class Reciprocal : public sw::Node
{
public:
	Reciprocal()
		: Node("Reciprocal")
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
		return "#_out# = 1.0 / #_in#;\n";
	}

}; // Reciprocal

}
}
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
			{ 0, "in_" },
		}, {
			{ 0, "out_" },
		}, {
		});

		m_dim_group = { 0, MAX_IMPORTS_COUNT };
	}

protected:
	virtual std::string GetBody() const override
	{
		return "(out_) = 1.0 / (in_);\n";
	}

}; // Reciprocal

}
}
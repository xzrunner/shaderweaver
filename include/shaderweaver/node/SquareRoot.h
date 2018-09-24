#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class SquareRoot : public sw::Node
{
public:
	SquareRoot()
		: Node("SquareRoot")
	{
		InitVariables({
			{ 0, "input" },
		}, {
			{ 0, "square" },
		}, {
		});

		m_dim_group = { 0, MAX_IMPORTS_COUNT };
	}

protected:
	virtual std::string GetBody() const override
	{
		return "(square) = sqrt((input));\n";
	}

}; // SquareRoot

}
}
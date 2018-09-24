#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class Divide : public sw::Node
{
public:
	Divide()
		: Node("Divide")
	{
		InitVariables({
			{ 0,  "a" },
			{ 0,  "b" },
		}, {
			{ 0, "div" },
		}, {
		});

		m_dim_group = { 0, 1, MAX_IMPORTS_COUNT };
	}

	enum InputID
	{
		ID_A = 0,
		ID_B,
	};

protected:
	virtual std::string GetBody() const override
	{
		return "(div) = (a) / (b);\n";
	}

}; // Divide

}
}
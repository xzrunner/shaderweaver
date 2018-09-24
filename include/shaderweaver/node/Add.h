#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class Add : public sw::Node
{
public:
	Add()
		: Node("Add")
	{
		InitVariables({
			{ 0,  "a" },
			{ 0,  "b" },
		}, {
			{ 0, "add" },
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
		return "(add) = (a) + (b);\n";
	}

}; // Add

}
}
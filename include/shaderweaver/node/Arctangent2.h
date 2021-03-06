#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"

namespace sw
{
namespace node
{

class Arctangent2 : public sw::Node
{
public:
	Arctangent2()
		: Node("Arctangent2")
	{
		InitVariables({
			{ 0, "a" },
			{ 0, "b" },
		}, {
			{ 0, "_out" },
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
		return "#_out# = atan(#a#, #b#);\n";
	}

	RTTR_ENABLE(Node)

}; // Arctangent2

}
}
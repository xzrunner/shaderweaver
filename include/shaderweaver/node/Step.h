#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"

namespace sw
{
namespace node
{

class Step : public sw::Node
{
public:
	Step()
		: Node("Step")
	{
		InitVariables({
			{ 0, "_in" },
			{ 0, "edge" },
		}, {
			{ 0, "_out" },
		}, {
		});

		m_dim_group = { 0, 1, MAX_IMPORTS_COUNT };
	}

	enum InputID
	{
		ID_INPUT = 0,
		ID_EDGE,
	};

protected:
	virtual std::string GetBody() const override
	{
		return "#_out# = step(#edge#, #_in#);\n";
	}

	RTTR_ENABLE(Node)

}; // Step

}
}
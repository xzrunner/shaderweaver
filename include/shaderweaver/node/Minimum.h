#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"

namespace sw
{
namespace node
{

class Minimum : public sw::Node
{
public:
	Minimum()
		: Node("Minimum")
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
		return "#_out# = min(#a#, #b#);\n";
	}

	RTTR_ENABLE(Node)

}; // Minimum

}
}
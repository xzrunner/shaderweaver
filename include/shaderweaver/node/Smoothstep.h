#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class Smoothstep : public sw::Node
{
public:
	Smoothstep()
		: Node("Smoothstep")
	{
		InitVariables({
			{ 0,  "a" },
			{ 0,  "b" },
			{ 0,  "t" },
		}, {
			{ 0, "_out" },
		}, {
		});

		m_dim_group = { ID_A, ID_B, ID_T, MAX_IMPORTS_COUNT };
	}

	enum InputID
	{
		ID_A = 0,
		ID_B,
		ID_T,
	};

protected:
	virtual std::string GetBody() const override
	{
		return "#_out# = smoothstep(#a#, #b#, #t#);\n";
	}

	RTTR_ENABLE(Node)

}; // Smoothstep

}
}
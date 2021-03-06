#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class Lerp : public sw::Node
{
public:
	Lerp()
		: Node("Lerp")
	{
		InitVariables({
			{ 0,  "a" },
			{ 0,  "b" },
			{ 0,  "t" },
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
		ID_T,
	};

protected:
	virtual std::string GetBody() const override
	{
		return "#_out# = mix(#a#, #b#, #t#);\n";
	}

	RTTR_ENABLE(Node)

}; // Lerp

}
}
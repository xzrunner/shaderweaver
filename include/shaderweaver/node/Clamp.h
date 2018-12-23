#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"

namespace sw
{
namespace node
{

class Clamp : public sw::Node
{
public:
	Clamp()
		: Node("Clamp")
	{
		InitVariables({
			{ 0, "_in" },
			{ 0, "minimum" },
			{ 0, "maximum" },
		}, {
			{ 0, "_out" },
		}, {
		});

		m_dim_group = { ID_IN, ID_MIN, ID_MAX, MAX_IMPORTS_COUNT };
	}

	enum InputID
	{
		ID_IN = 0,
		ID_MIN,
		ID_MAX
	};

protected:
	virtual std::string GetBody() const override
	{
		return "#_out# = clamp(#_in#, #minimum#, #maximum#);\n";
	}

	RTTR_ENABLE(Node)

}; // Clamp

}
}
#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class Distance : public sw::Node
{
public:
	Distance()
		: Node("Distance")
	{
		InitVariables({
			{ 0, "a" },
			{ 0, "b" },
		}, {
			{ t_flt1, "_out" },
		}, {
		});

		m_dim_group = { 0, 1 };
	}

	enum InputID
	{
		ID_A = 0,
		ID_B,
	};

protected:
	virtual std::string GetBody() const override
	{
		return "#_out# = distance(#a#, #b#);\n";
	}

	RTTR_ENABLE(Node)

}; // Distance

}
}
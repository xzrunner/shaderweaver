#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"

namespace sw
{
namespace node
{

class Projection : public sw::Node
{
public:
	Projection()
		: Node("Projection")
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
		return "#_out# = #b# * dot(#a#, #b#) / dot(#b#, #b#);\n";
	}

	RTTR_ENABLE(Node)

}; // Projection

}
}
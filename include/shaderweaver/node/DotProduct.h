#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class DotProduct : public sw::Node
{
public:
	DotProduct()
		: Node("DotProduct")
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
		return "#_out# = dot(#a#, #b#);\n";
	}

}; // DotProduct

}
}
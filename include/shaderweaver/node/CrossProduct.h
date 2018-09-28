#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class CrossProduct : public sw::Node
{
public:
	CrossProduct()
		: Node("CrossProduct")
	{
		InitVariables({
			{ t_flt3, "a" },
			{ t_flt3, "b" },
		}, {
			{ t_flt3, "_out" },
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
		return "#_out# = cross(#a#, #b#);\n";
	}

}; // CrossProduct

}
}
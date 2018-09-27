#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class Branch : public sw::Node
{
public:
	Branch()
		: Node("Branch")
	{
		InitVariables({
			{ t_bool, "predicate" },
			{ 0,      "true" },
			{ 0,      "false" },
		}, {
			{ 0,      "_out" },
		}, {
		});

		m_dim_group = { ID_TRUE, ID_FALSE, MAX_IMPORTS_COUNT };
	}

	enum InputID
	{
		ID_PREDICATE = 0,
		ID_TRUE,
		ID_FALSE,
	};

protected:
	virtual std::string GetBody() const override
	{
		return "#_out# = mix(#false#, #true#, int(#predicate#));\n";
	}

}; // Branch

}
}
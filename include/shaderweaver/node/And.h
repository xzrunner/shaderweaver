#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class And : public sw::Node
{
public:
	And()
		: Node("And")
	{
		InitVariables({
			{ t_bool,  "a" },
			{ t_bool,  "b" },
		}, {
			{ t_bool, "_out" },
		}, {
		});
	}

	enum InputID
	{
		ID_A = 0,
		ID_B,
	};

protected:
	virtual std::string GetBody() const override
	{
		return "#_out# = #a# && #b#;\n";
	}

	RTTR_ENABLE(Node)

}; // And

}
}
#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class Absolute : public sw::Node
{
public:
	Absolute()
		: Node("Absolute")
	{
		InitVariables({
			{ 0, "abs_in" },
		}, {
			{ 0, "abs_out" },
		}, {
		});
	}

	virtual void Update() override {
		NodeHelper::TypePropote(*this);
	}

protected:
	virtual std::string GetBody() const override
	{
		return "(abs_out) = abs((abs_in));\n";
	}

}; // Absolute

}
}
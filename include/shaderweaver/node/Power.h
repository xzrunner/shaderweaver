#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class Power : public sw::Node
{
public:
	Power()
		: Node("Power")
	{
		InitVariables({
			{ 0,  "a" },
			{ 0,  "b" },
		}, {
			{ 0, "power" },
		}, {
		});
	}

	virtual void Update() override {
		NodeHelper::TypePropote(*this);
	}

	enum InputID
	{
		ID_A = 0,
		ID_B,
	};

protected:
	virtual std::string GetBody() const override
	{
		return "(power) = pow((a), (b));\n";
	}

}; // Power

}
}
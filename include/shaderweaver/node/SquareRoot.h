#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class SquareRoot : public sw::Node
{
public:
	SquareRoot()
		: Node("SquareRoot")
	{
		InitVariables({
			{ 0, "input" },
		}, {
			{ 0, "square" },
		}, {
		});
	}

	virtual void Update() override {
		NodeHelper::TypePropote(*this);
	}

protected:
	virtual std::string GetBody() const override
	{
		return "(square) = sqrt((input));\n";
	}

}; // SquareRoot

}
}
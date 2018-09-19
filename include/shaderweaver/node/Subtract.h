#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class Subtract : public sw::Node
{
public:
	Subtract()
		: Node("Subtract")
	{
		InitVariables({
			{ 0,  "a" },
			{ 0,  "b" },
		}, {
			{ 0, "sub" },
		}, {
		});
	}

	virtual void Update() override {
		NodeHelper::TypePropote(*this);
	}

	enum ImportIdx
	{
		IN_A = 0,
		IN_B,
	};

protected:
	virtual std::string GetBody() const override
	{
		return "(sub) = (a) - (b);\n";
	}

}; // Subtract

}
}
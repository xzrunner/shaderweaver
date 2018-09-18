#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class Multiply : public sw::Node
{
public:
	Multiply()
		: Node("Multiply")
	{
		AddVariable(Variable(t_n_in,  "a"));
		AddVariable(Variable(t_n_in,  "b"));

		AddVariable(Variable(t_n_out, "mul"));
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
		return "(mul) = (a) * (b);\n";
	}

}; // Multiply

}
}
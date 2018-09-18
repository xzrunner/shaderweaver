#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class InverseLerp : public sw::Node
{
public:
	InverseLerp()
		: Node("InverseLerp")
	{
		AddVariable(Variable(t_n_in,  "a"));
		AddVariable(Variable(t_n_in,  "b"));
		AddVariable(Variable(t_n_in,  "t"));

		AddVariable(Variable(t_n_out, "inv_lerp"));
	}

	virtual void Update() override {
		NodeHelper::TypePropote(*this);
	}

	enum ImportIdx
	{
		IN_A = 0,
		IN_B,
		IN_T,
	};

protected:
	virtual std::string GetBody() const override
	{
		return "(inv_lerp) = ((t) - (a)) / ((b) - (a));\n";
	}

}; // InverseLerp

}
}
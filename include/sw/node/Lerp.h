#pragma once

#include "sw/Node.h"
#include "sw/NodeHelper.h"
#include "sw/VariableType.h"

namespace sw
{
namespace node
{

class Lerp : public sw::Node
{
public:
	Lerp()
		: Node("Lerp")
	{
		AddVariable(Variable(t_n_in,  "a"));
		AddVariable(Variable(t_n_in,  "b"));
		AddVariable(Variable(t_n_in,  "t"));

		AddVariable(Variable(t_n_out, "lerp"));
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
		return "(lerp) = mix((a), (b), (t));\n";
	}

}; // Lerp

}
}
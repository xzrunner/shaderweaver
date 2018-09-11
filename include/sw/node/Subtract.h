#pragma once

#include "sw/Node.h"
#include "sw/NodeHelper.h"
#include "sw/VariableType.h"

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
		AddVariable(Variable(t_n_in,  "a"));
		AddVariable(Variable(t_n_in,  "b"));

		AddVariable(Variable(t_n_out, "sub"));
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
		return "(add) = (a) - (b);\n";
	}

}; // Subtract

}
}
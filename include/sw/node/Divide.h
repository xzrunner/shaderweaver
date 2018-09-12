#pragma once

#include "sw/Node.h"
#include "sw/NodeHelper.h"
#include "sw/VariableType.h"

namespace sw
{
namespace node
{

class Divide : public sw::Node
{
public:
	Divide()
		: Node("Divide")
	{
		AddVariable(Variable(t_n_in,  "a"));
		AddVariable(Variable(t_n_in,  "b"));

		AddVariable(Variable(t_n_out, "div"));
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
		return "(div) = (a) / (b);\n";
	}

}; // Divide

}
}
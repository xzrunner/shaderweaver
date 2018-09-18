#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class Add : public sw::Node
{
public:
	Add()
		: Node("Add")
	{
		AddVariable(Variable(t_n_in,  "a"));
		AddVariable(Variable(t_n_in,  "b"));

		AddVariable(Variable(t_n_out, "add"));
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
		return "(add) = (a) + (b);\n";
	}

}; // Add

}
}
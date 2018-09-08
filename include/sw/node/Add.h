#pragma once

#include "sw/Node.h"
#include "sw/VariableType.h"

namespace sw
{
namespace node
{

class Add : public Node
{
public:
	Add()
		: Node("Add")
	{
		AddVariable(Variable(t_in,  "a"));
		AddVariable(Variable(t_in,  "b"));

		AddVariable(Variable(t_out, "add"));
	}

	virtual void Update() override;

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
#pragma once

#include "sw/Node.h"
#include "sw/VariableType.h"

namespace sw
{
namespace node
{

class Gray : public Node
{
public:
	Gray()
		: Node("Gray")
	{
		AddVariable(Variable(t_col4 | t_n_in, "col4"));
		AddVariable(Variable(t_flt1 | t_n_mid, "flt1"));

		AddVariable(Variable(t_col4 | t_n_out, "gray"));
	}

protected:
	virtual std::string GetBody() const override
	{
		return R"(
(flt1) = dot((col4).rgb , vec3(0.299, 0.587, 0.114));
(gray) = vec4((flt1), (flt1), (flt1), (col4).a);
)" + 1;
	}

}; // Gray

}
}
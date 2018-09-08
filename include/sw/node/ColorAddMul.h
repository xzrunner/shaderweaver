#pragma once

#include "sw/Node.h"
#include "sw/VariableType.h"

namespace sw
{
namespace node
{

class ColorAddMul : public Node
{
public:
	ColorAddMul()
		: Node("ColorAddMul")
	{
		AddVariable(Variable(t_col4 | t_n_in, "col"));
		AddVariable(Variable(t_col4 | t_n_in, "mul"));
		AddVariable(Variable(t_col4 | t_n_in, "add"));

		AddVariable(Variable(t_col4 | t_n_out, "col_add_mul"));
	}

	enum ImportIdx
	{
		IN_COL = 0,
		IN_MUL,
		IN_ADD,
	};

protected:
	virtual std::string GetBody() const override
	{
		return R"(
(col_add_mul).xyz   = (col).xyz * (mul).xyz;
(col_add_mul).w     = (col).w;
(col_add_mul)      *= (mul).w;
(col_add_mul).xyz  += (add).xyz * (col).w * (mul).w;
(col_add_mul).xyzw += (add).w * 10.0 * (col_add_mul).xyzw;
)" + 1;
	}

}; // ColorAddMul

}
}
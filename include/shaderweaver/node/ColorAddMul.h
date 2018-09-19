#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class ColorAddMul : public sw::Node
{
public:
	ColorAddMul()
		: Node("ColorAddMul")
	{
		InitVariables({
			{ t_col4, "col" },
			{ t_col4, "mul" },
			{ t_col4, "add" },
		}, {
			{ t_col4, "col_add_mul" },
		}, {
		});
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
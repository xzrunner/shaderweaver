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
			{ t_col4, "_in" },
			{ t_col4, "mul" },
			{ t_col4, "add" },
		}, {
			{ t_col4, "_out" },
		}, {
		});
	}

	enum InputID
	{
		ID_COL = 0,
		ID_MUL,
		ID_ADD,
	};

protected:
	virtual std::string GetBody() const override
	{
		return R"(
#_out#.xyz   = #_in#.xyz * #mul#.xyz;
#_out#.w     = #_in#.w;
#_out#      *= #mul#.w;
#_out#.xyz  += #add#.xyz * #_in#.w * #mul#.w;
#_out#.xyzw += #add#.w * 10.0 * #_out#.xyzw;
)" + 1;
	}

}; // ColorAddMul

}
}
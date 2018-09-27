#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class Ellipse : public Node
{
public:
	Ellipse()
		: Node("Ellipse")
	{
		InitVariables({
			{ t_uv, "uv" },
			{ t_flt1, "width" },
			{ t_flt1, "height" },
		}, {
			{ t_flt1, "_out" },
		}, {
			{ t_flt1, "d" },
		});
	}


	enum InputID
	{
		ID_UV = 0,
		ID_WIDTH,
		ID_HEIGHT,
	};

protected:
	virtual std::string GetBody() const override
	{
		return R"(
#d# = length((#uv# * 2 - 1) / vec2(#width#, #height#));
#_out# = clamp((1 - #d#) / fwidth(#d#), 0.0, 1.0);
)" + 1;
	}

}; // Ellipse

}
}
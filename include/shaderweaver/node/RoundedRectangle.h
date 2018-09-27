#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class RoundedRectangle : public Node
{
public:
	RoundedRectangle()
		: Node("RoundedRectangle")
	{
		InitVariables({
			{ t_uv, "uv" },
			{ t_flt1, "width" },
			{ t_flt1, "height" },
			{ t_flt1, "radius" },
		}, {
			{ t_flt1, "_out" },
		}, {
			{ t_flt1, "new_radius" },
			{ t_flt2, "new_uv" },
			{ t_flt1, "d" },
		});
	}


	enum InputID
	{
		ID_UV = 0,
		ID_WIDTH,
		ID_HEIGHT,
		ID_RADIUS,
	};

protected:
	virtual std::string GetBody() const override
	{
		return R"(
#new_radius# = max(min(min(abs(#radius# * 2), abs(#width#)), abs(#height#)), 1e-5);
#new_uv# = abs(#uv# * 2 - 1) - vec2(#width#, #height#) + #new_radius#;
#d# = length(max(vec2(0, 0), #new_uv#)) / #new_radius#;
#_out# = clamp((1 - #d#) / fwidth(#d#), 0.0, 1.0);
)" + 1;
	}

}; // RoundedRectangle

}
}
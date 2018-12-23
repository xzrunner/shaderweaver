#pragma once

#include "shaderweaver/Node.h"

namespace sw
{
namespace node
{

class PolarCoordinates : public sw::Node
{
public:
	PolarCoordinates()
		: Node("PolarCoordinates")
	{
		InitVariables({
			{ t_uv, "uv" },
			{ t_flt2, "center" },
			{ t_flt1, "radial_scale" },
			{ t_flt1, "length_scale" },
		}, {
			{ t_uv, "_out" },
		}, {
			{ t_flt2, "delta" },
			{ t_flt1, "radius" },
			{ t_flt1, "angle" },
		});
	}

	enum InputID
	{
		ID_UV = 0,
		ID_CENTER,
		ID_RADIAL_SCALE,
		ID_LENGTH_SCALE,
	};

protected:
	virtual std::string GetBody() const override
	{
		return R"(
#delta# = #uv# - #center#;
#radius# = length(#delta#) * 2 * #radial_scale#;
#angle# = atan(#delta#.x, #delta#.y) * 1.0/6.28 * #length_scale#;
#_out# = vec2(#radius#, #angle#);
)" + 1;
	}

	RTTR_ENABLE(Node)

}; // PolarCoordinates

}
}
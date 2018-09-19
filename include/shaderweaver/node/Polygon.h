#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class Polygon : public Node
{
public:
	Polygon()
		: Node("Polygon")
	{
		InitVariables({
			{ t_uv, "uv" },
			{ t_flt1, "sides" },
			{ t_flt1, "width" },
			{ t_flt1, "height" },
		}, {
			{ t_flt1, "polygon" },
		}, {
			{ t_flt1, "tau" },
			{ t_flt2, "_uv" },
			{ t_flt1, "coord" },
			{ t_flt1, "r" },
			{ t_flt1, "distance" },
		});
	}


	enum ImportIdx
	{
		IN_UV = 0,
		IN_SIDES,
		ID_WIDTH,
		ID_HEIGHT,
	};

protected:
	virtual std::string GetBody() const override
	{
		return R"(
(tau) = 6.28318530718;
(_uv) = ((uv) * 2 - 1) / vec2((width), (height));
(_uv).y *= -1;
(coord) = atan((_uv).x, (_uv).y);
(r) = (tau) / (sides);
(distance) = cos(floor(0.5 + (coord) / (r)) * (r) - (coord)) * length((_uv));
(polygon) = clamp((1 - (distance)) / fwidth((distance)), 0.0, 1.0);
)" + 1;
	}

}; // Polygon

}
}
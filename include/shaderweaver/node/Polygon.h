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
		AddVariable(Variable(t_uv | t_n_in, "uv"));
		AddVariable(Variable(t_flt1 | t_n_in, "sides"));
		AddVariable(Variable(t_flt1 | t_n_in, "width"));
		AddVariable(Variable(t_flt1 | t_n_in, "height"));

		AddVariable(Variable(t_flt1 | t_n_out, "polygon"));

		AddVariable(Variable(t_flt1 | t_n_mid, "tau"));
		AddVariable(Variable(t_flt2 | t_n_mid, "_uv"));
		AddVariable(Variable(t_flt1 | t_n_mid, "coord"));
		AddVariable(Variable(t_flt1 | t_n_mid, "r"));
		AddVariable(Variable(t_flt1 | t_n_mid, "distance"));
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
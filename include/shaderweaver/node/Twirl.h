#pragma once

#include "shaderweaver/Node.h"

namespace sw
{
namespace node
{

class Twirl : public sw::Node
{
public:
	Twirl()
		: Node("Twirl")
	{
		AddVariable(Variable(t_uv | t_n_in, "uv"));
		AddVariable(Variable(t_flt2 | t_n_in, "center"));
		AddVariable(Variable(t_flt1 | t_n_in, "strength"));
		AddVariable(Variable(t_flt2 | t_n_in, "offset"));

		AddVariable(Variable(t_uv | t_n_out, "twirl"));

		AddVariable(Variable(t_flt2 | t_n_mid, "delta"));
		AddVariable(Variable(t_flt1 | t_n_mid, "angle"));
		AddVariable(Variable(t_flt1 | t_n_mid, "x"));
		AddVariable(Variable(t_flt1 | t_n_mid, "y"));
	}

	enum ImportIdx
	{
		IN_UV = 0,
		IN_CENTER,
		IN_STRENGTH,
		IN_OFFSET,
	};

protected:
	virtual std::string GetBody() const override
	{
		return R"(
(delta) = (uv) - (center);
(angle) = (strength) * length((delta));
(x) = cos((angle)) * (delta).x - sin((angle)) * (delta).y;
(y) = sin((angle)) * (delta).x + cos((angle)) * (delta).y;
(twirl) = vec2((x) + (center).x + (offset).x, (y) + (center).y + (offset).y);
)" + 1;
	}

}; // Twirl

}
}
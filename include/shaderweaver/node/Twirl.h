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
		InitVariables({
			{ t_uv, "uv" },
			{ t_flt2, "center" },
			{ t_flt1, "strength" },
			{ t_flt2, "offset" },
		}, {
			{ t_uv, "twirl" },
		}, {
			{ t_flt2, "delta" },
			{ t_flt1, "angle" },
			{ t_flt1, "x" },
			{ t_flt1, "y" },
		});
	}

	enum InputID
	{
		ID_UV = 0,
		ID_CENTER,
		ID_STRENGTH,
		ID_OFFSET,
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
#pragma once

#include "shaderweaver/Node.h"

namespace sw
{
namespace node
{

class RadialShear : public sw::Node
{
public:
	RadialShear()
		: Node("RadialShear")
	{
		InitVariables({
			{ t_uv, "uv" },
			{ t_flt2, "center" },
			{ t_flt2, "strength" },
			{ t_flt2, "offset" },
		}, {
			{ t_uv, "_out" },
		}, {
			{ t_flt2, "delta" },
			{ t_flt1, "delta2" },
			{ t_flt2, "delta_offset" },
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
#delta# = #uv# - #center#;
#delta2# = dot(#delta#.xy, #delta#.xy);
#delta_offset# = #delta2# * #strength#;
#_out# = #uv# + vec2(#delta#.y, -#delta#.x) * #delta_offset# + #offset#;
)" + 1;
	}

	RTTR_ENABLE(Node)

}; // RadialShear

}
}
#pragma once

#include "shaderweaver/Node.h"

namespace sw
{
namespace node
{

class Spherize : public sw::Node
{
public:
	Spherize()
		: Node("Spherize")
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
			{ t_flt1, "delta4" },
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
#delta4# = #delta2# * #delta2#;
#delta_offset# = #delta4# * #strength#;
#_out# = #uv# + #delta# * #delta_offset# + #offset#;
)" + 1;
	}

}; // Spherize

}
}
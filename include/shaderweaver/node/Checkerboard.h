#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class Checkerboard : public Node
{
public:
	Checkerboard()
		: Node("Checkerboard")
	{
		InitVariables({
			{ t_uv, "uv" },
			{ t_col3, "col_a" },
			{ t_col3, "col_b" },
			{ t_flt2, "frequency" },
		}, {
			{ t_col3, "_out" },
		}, {
			{ t_flt2, "new_uv" },
			{ t_flt4, "derivatives" },
			{ t_flt2, "duv_length" },
			{ t_flt1, "width" },
			{ t_flt2, "distance3" },
			{ t_flt2, "scale" },
			{ t_flt2, "blend_out" },
			{ t_flt2, "vector_alpha" },
			{ t_flt1, "alpha" },
		});
	}

	enum InputID
	{
		ID_UV = 0,
		ID_COLOR_A,
		ID_COLOR_B,
		ID_FREQUENCY,
	};

protected:
	virtual std::string GetBody() const override
	{
		return R"(
#new_uv# = #uv# + 0.25 / #frequency#;
#derivatives# = vec4(dFdx(#new_uv#), dFdy(#new_uv#));
#duv_length# = sqrt(vec2(dot(#derivatives#.xz, #derivatives#.xz), dot(#derivatives#.yw, #derivatives#.yw)));
#width# = 0.5;
#distance3# = 2.0 * abs(fract((#new_uv#.xy + 0.5) * #frequency#) - 0.5) - #width#;
#scale# = 0.5 / #duv_length#.xy;
#blend_out# = clamp(#scale# / 3, 0.0, 1.0);
#vector_alpha# = clamp(#distance3# * #scale#.xy * #blend_out#.xy, -1.0, 1.0);
#alpha# = clamp(#vector_alpha#.x * #vector_alpha#.y, 0.0, 1.0);
#_out# = mix(#col_a#, #col_b#, #alpha#);
)" + 1;
	}

	RTTR_ENABLE(Node)

}; // Checkerboard

}
}
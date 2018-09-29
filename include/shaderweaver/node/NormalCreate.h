#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"

namespace sw
{
namespace node
{

class NormalCreate : public sw::Node
{
public:
	NormalCreate()
		: Node("NormalCreate")
	{
		InitVariables({
			{ t_tex2d, "tex" },
			{ t_uv,    "uv" },
			{ t_flt1,  "offset" },
			{ t_flt1,  "strength" },
		}, {
			{ t_flt3,  "_out" },
		}, {
			{ t_flt1,  "offset_new" },
			{ t_flt2,  "offset_u" },
			{ t_flt2,  "offset_v" },
			{ t_flt1,  "normal_sample" },
			{ t_flt1,  "u_sample" },
			{ t_flt1,  "v_sample" },
			{ t_flt3,  "va" },
			{ t_flt3,  "vb" },
		});
	}

	enum InputID
	{
		ID_TEX = 0,
		ID_UV,
		ID_OFFSET,
		ID_STRENGTH,
	};

protected:
	virtual std::string GetBody() const override
	{
		return R"(
#offset_new# = pow(#offset#, 3) * 0.1;
#offset_u# = vec2(#uv#.x + #offset_new#, #uv#.y);
#offset_v# = vec2(#uv#.x, #uv#.y + #offset_new#);
#normal_sample# = texture2D(#tex#, #uv#).x;
#u_sample# = texture2D(#tex#, #offset_u#).x;
#v_sample# = texture2D(#tex#, #offset_v#).x;
#va# = vec3(1, 0, (#u_sample# - #normal_sample#) * #strength#);
#vb# = vec3(0, 1, (#v_sample# - #normal_sample#) * #strength#);
#_out# = normalize(cross(#va#, #vb#));
)" + 1;
	}

}; // NormalCreate

}
}
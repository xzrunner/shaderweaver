#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"

namespace sw
{
namespace node
{

class NormalUnpack : public sw::Node
{
public:
	NormalUnpack()
		: Node("NormalUnpack")
	{
		InitVariables({
			{ t_flt4, "_in" },
		}, {
			{ t_flt3, "_out" },
		}, {
			{ t_flt4, "packed_normal" },
		});
	}

protected:
	virtual std::string GetBody() const override
	{
		return R"(
#packed_normal# = #_in#;
#packed_normal#.w *= #packed_normal#.x;
#_out#.xy = #packed_normal#.wy * 2.0 - 1.0;
#_out#.z = sqrt(1.0 - clamp(dot(#_out#.xy, #_out#.xy), 0.0, 1.0));
)" + 1;
	}

}; // NormalUnpack

}
}
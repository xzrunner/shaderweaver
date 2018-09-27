#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class Gray : public sw::Node
{
public:
	Gray()
		: Node("Gray")
	{
		InitVariables({
			{ t_col4, "_in" },
		}, {
			{ t_col4, "_out" },
		}, {
			{ t_flt1, "flt1" },
		});
	}

protected:
	virtual std::string GetBody() const override
	{
		return R"(
#flt1# = dot(#_in#.rgb , vec3(0.299, 0.587, 0.114));
#_out# = vec4(#flt1#, #flt1#, #flt1#, #_in#.a);
)" + 1;
	}

}; // Gray

}
}
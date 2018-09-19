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
			{ t_col4, "col4" },
		}, {
			{ t_col4, "gray" },
		}, {
			{ t_flt1, "flt1" },
		});
	}

protected:
	virtual std::string GetBody() const override
	{
		return R"(
(flt1) = dot((col4).rgb , vec3(0.299, 0.587, 0.114));
(gray) = vec4((flt1), (flt1), (flt1), (col4).a);
)" + 1;
	}

}; // Gray

}
}
#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class Saturation : public sw::Node
{
public:
	Saturation()
		: Node("Saturation")
	{
		InitVariables({
			{ t_flt3, "_in" },
			{ t_flt1, "saturation" },
		}, {
			{ t_flt3, "_out" },
		}, {
			{ t_flt1, "luma" },
		});
	}

	enum InputID
	{
		ID_INPUT = 0,
		ID_SATURATION,
	};

protected:
	virtual std::string GetBody() const override
	{
		return R"(
#luma# = dot(#_in#, vec3(0.2126729, 0.7151522, 0.0721750));
#_out# = vec3(#luma#, #luma#, #luma#) + vec3(#saturation#, #saturation#, #saturation#) * (#_in# - vec3(#luma#, #luma#, #luma#));
)" + 1;
	}

	RTTR_ENABLE(Node)

}; // Saturation

}
}
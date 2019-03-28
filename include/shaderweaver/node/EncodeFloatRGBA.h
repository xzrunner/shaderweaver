#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class EncodeFloatRGBA : public sw::Node
{
public:
	EncodeFloatRGBA()
		: Node("EncodeFloatRGBA")
	{
		InitVariables({
			{ t_flt1, "_in" },
		}, {
			{ t_flt4, "rgba" },
		}, {
		});
	}

	RTTR_ENABLE(Node)

}; // EncodeFloatRGBA

}
}
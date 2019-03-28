#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class DecodeFloatRGBA : public sw::Node
{
public:
	DecodeFloatRGBA()
		: Node("DecodeFloatRGBA")
	{
		InitVariables({
			{ t_flt4, "rgba" },
		}, {
			{ t_flt1, "_out" },
		}, {
		});
	}

	RTTR_ENABLE(Node)

}; // DecodeFloatRGBA

}
}
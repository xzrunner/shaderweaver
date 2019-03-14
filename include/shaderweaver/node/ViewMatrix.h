#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class ViewMatrix : public Node
{
public:
	ViewMatrix()
		: Node("ViewMatrix")
	{
		InitVariables({
		}, {
			{ t_mat4, "_out" },
		}, {
		});
	}

	RTTR_ENABLE(Node)

}; // ViewMatrix

}
}
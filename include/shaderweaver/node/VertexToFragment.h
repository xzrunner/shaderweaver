#pragma once

#include "shaderweaver/Node.h"

namespace sw
{
namespace node
{

class VertexToFragment : public sw::Node
{
public:
	VertexToFragment()
		: Node("VertexToFragment")
	{
		InitVariables({
			{ 0, "_in" },
		}, {
			{ 0, "_out" },
		}, {
		});
	}

	RTTR_ENABLE(Node)

}; // VertexToFragment

}
}
#pragma once

#include "shaderweaver/Node.h"

namespace sw
{

class NodeHelper
{
public:
	static void TypePropote(const Node::PortAddr& from,
		const Node::PortAddr& to);

}; // NodeHelper

}
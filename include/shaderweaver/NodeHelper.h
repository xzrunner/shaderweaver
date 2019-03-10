#pragma once

#include "shaderweaver/Node.h"

namespace sw
{

class Node;

class NodeHelper
{
public:
	static void TypePromote(const Node::PortAddr& from,
		const Node::PortAddr& to);

    static void SetImports(Node& node, const std::vector<Variable>& vars);
    static void SetExports(Node& node, const std::vector<Variable>& vars);

}; // NodeHelper

}
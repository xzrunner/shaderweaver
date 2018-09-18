#include "shaderweaver/NodeHelper.h"
#include "shaderweaver/Node.h"

namespace sw
{

void NodeHelper::TypePropote(Node& node)
{
	auto& imports = node.GetImports();
	bool empty = true;
	for (auto& p : imports) {
		if (!p.conns.empty()) {
			empty = false;
			break;
		}
	}

	VariableType type;
	for (auto& p : imports)
	{
		auto in_port = p.GetPair(0);
		if (!in_port) {
			continue;
		}
		type.TypePromoteFrom(in_port->var.Type());
	}

	for (auto& p : imports) {
		const_cast<sw::Node::Port&>(p).var.Type().TypePromoteFrom(type);
	}
	for (auto& p : node.GetExports()) {
		const_cast<sw::Node::Port&>(p).var.Type().TypePromoteFrom(type);
	}
}

}
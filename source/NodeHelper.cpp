#include "shaderweaver/NodeHelper.h"

#include <algorithm>

#include <assert.h>

namespace sw
{

void NodeHelper::TypePromote(const Node::PortAddr& from, const Node::PortAddr& to)
{
	auto f_node = from.node.lock();
	assert(f_node && from.idx >= 0 && from.idx < static_cast<int>(f_node->GetExports().size()));
	auto& f_var = f_node->GetExports()[from.idx].var;

	auto t_node = to.node.lock();
	assert(t_node && to.idx >= 0 && to.idx < static_cast<int>(t_node->GetImports().size()));
	auto& t_var = t_node->GetImports()[to.idx].var;

	auto f_type = f_var.GetType();
	auto t_type = t_var.GetType();

	if (t_var.IsDimDynamic() && f_type.dim > t_type.dim)
	{
		t_type.dim = f_type.dim;
		const_cast<sw::Variable&>(t_var).SetType(t_type);
	}

	if (f_type.precision > t_type.precision)
	{
		t_type.precision = f_type.precision;
		const_cast<sw::Variable&>(t_var).SetType(t_type);
	}

	// update dim group
	auto& dim_group = t_node->GetDimGroup();
	if (!dim_group.empty())
	{
		bool find = false;
		for (auto& idx : dim_group) {
			if (idx == to.idx) {
				find = true;
				break;
			}
		}
		if (find)
		{
			for (auto& idx : dim_group)
			{
				auto& port = idx < Node::MAX_IMPORTS_COUNT ?
					t_node->GetImports()[idx] : t_node->GetExports()[idx - Node::MAX_IMPORTS_COUNT];
				auto type = port.var.GetType();
				type.dim = std::max(type.dim, t_type.dim);
				type.precision = std::max(type.precision, t_type.precision);
				const_cast<Node::Port&>(port).var.SetType(type);
			}
		}
	}
}

void NodeHelper::SetImports(Node& node, const std::vector<Variable>& vars)
{
    std::vector<Node::Port> imports;

    imports.reserve(vars.size());
    for (auto& var : vars)
    {
        auto v = var;
        auto type = v.GetType();
        type.region = VT_NODE_IN;
        v.SetType(type);
        imports.push_back(v);
    }

    node.SetImports(imports);
}

void NodeHelper::SetExports(Node& node, const std::vector<Variable>& vars)
{
    std::vector<Node::Port> exports;

    exports.reserve(vars.size());
    for (auto& var : vars)
    {
        auto v = var;
        auto type = v.GetType();
        type.region = VT_NODE_OUT;
        v.SetType(type);
        exports.push_back(v);
    }

    node.SetExports(exports);
}

}
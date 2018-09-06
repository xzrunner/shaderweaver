#include "sw/Evaluator.h"
#include "sw/Node.h"
#include "sw/VariableType.h"
#include "sw/node/Varying.h"

#include <cpputil/StringHelper.h>

#include <map>

#include <assert.h>

namespace
{

const int SHADER_MAX_STR_LEN = 1024;

}

namespace sw
{

Evaluator::Evaluator(const NodePtr& vert, const NodePtr& frag)
{
	Flatten ft_frag(frag);
	Flatten ft_vert(vert);
	for (auto& node : ft_frag.nodes) {
		if (std::dynamic_pointer_cast<node::Varying>(node) != nullptr) {
			ft_vert.Insert(node);
		}
	}

	VariablesRename rn_vert(ft_vert);
	EvalDeclareOutside(m_vert, rn_vert, true);
	EvalBody(m_vert, rn_vert, ft_vert, true);

	VariablesRename rn_frag(ft_frag);
	EvalDeclareOutside(m_frag, rn_frag, false);
	EvalBody(m_frag, rn_frag, ft_frag, false);
}

void Evaluator::EvalDeclareOutside(std::string& dst, const VariablesRename& src, bool is_vert)
{
	auto& vars = src.GetRenamedVars();
	for (auto& var : vars)
	{
		auto& type = var.Type();
		if (type.qualifier == VT_TEMP) {
			continue;
		}
		if (!is_vert && type.qualifier == VT_ATTRIBUTE) {
			continue;
		}
		dst += cpputil::StringHelper::Format(
			"%s %s;\n", type.ToGLSL().c_str(), var.Name().c_str()
		);
	}
}

void Evaluator::EvalDeclareInside(std::string& dst, const VariablesRename& src)
{
	auto& vars = src.GetRenamedVars();
	for (auto& var : vars)
	{
		auto& type = var.Type();
		if (type.qualifier != VT_TEMP) {
			continue;
		}
		dst += cpputil::StringHelper::Format(
			"%s %s;\n", type.ToGLSL().c_str(), var.Name().c_str()
		);
	}
}

void Evaluator::EvalBody(std::string& dst, const VariablesRename& src_names,
	                     const Flatten& src, bool is_vert)
{
	std::string body;

	EvalDeclareInside(body, src_names);

	for (auto& itr = src.nodes.rbegin(); itr != src.nodes.rend(); ++itr) {
		if (!is_vert && std::dynamic_pointer_cast<node::Varying>(*itr) != nullptr) {
			continue;
		}
		body += "\n" + (*itr)->ToString() + "\n";
	}

	// final assign
	assert(!src.nodes.empty());
	auto& returns = src.nodes[0]->GetReturn();
	assert(returns.size() == 1 && returns[0].Type().dim == VT_4);
	if (is_vert) {
		body += cpputil::StringHelper::Format("gl_Position = %s;\n", returns[0].Name().c_str());
	} else {
		body += cpputil::StringHelper::Format("gl_FragColor = %s;\n", returns[0].Name().c_str());
	}

	dst += cpputil::StringHelper::Format(R"(
void main()
{
%s
}
	)", body.c_str());
}

//////////////////////////////////////////////////////////////////////////
// class Evaluator::Flatten
//////////////////////////////////////////////////////////////////////////

Evaluator::Flatten::Flatten(const NodePtr& node)
{
	Insert(node);
}

void Evaluator::Flatten::Insert(const NodePtr& node)
{
	size_t node_idx = nodes.size();
	size_t var_idx = vars.size();
	nodes.push_back(node);
	node2vars.insert({ node->GetID(), var_idx });

	for (auto& var : node->GetParams()) {
		vars.push_back({ var, node_idx });
	}
	for (auto& var : node->GetReturn()) {
		vars.push_back({ var, node_idx });
	}
}

//////////////////////////////////////////////////////////////////////////
// class Evaluator::VariablesRename
//////////////////////////////////////////////////////////////////////////

Evaluator::VariablesRename::VariablesRename(const Flatten& ft)
	: m_src(ft)
{
	Rename();
}

void Evaluator::VariablesRename::Rename()
{
	if (m_src.vars.empty()) {
		return;
	}

	m_src2dst.resize(m_src.vars.size());

	std::map<std::string, size_t> name2var;
	for (size_t i = 0, n = m_src.vars.size(); i < n; ++i)
	{
		auto& src = m_src.vars[i];
		auto itr = name2var.find(src.first.Name());
		if (itr == name2var.end())
		{
			size_t dst_idx = m_dst.size();
			m_src2dst[i] = dst_idx;
			name2var.insert({ src.first.Name(), dst_idx });
			m_dst.push_back(src.first);
		}
		else
		{
			if (m_src.vars[itr->second].first.Type() == src.first.Type())
			{
				m_src2dst[i] = itr->second;
			}
			else
			{
				size_t dst_idx = m_dst.size();
				Variable renamed(src.first.Type(), src.first.Name() + "_" + std::to_string(src.second));
				name2var.insert({ renamed.Name(), dst_idx });
				m_dst.push_back(renamed);
			}
		}
	}
}

}
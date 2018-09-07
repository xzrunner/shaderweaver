#include "sw/Evaluator.h"
#include "sw/Node.h"
#include "sw/VariableType.h"

#include <cpputil/StringHelper.h>

#include <map>

#include <assert.h>

namespace
{

const int SHADER_MAX_STR_LEN = 1024;

}

namespace sw
{

Evaluator::Evaluator(const std::vector<NodePtr>& nodes, ShaderType st)
	: m_st(st)
{
	InitNodes(nodes);
	Rename();
	EvalDeclareOutside();
	EvalBody();
}

void Evaluator::InitNodes(const std::vector<NodePtr>& nodes)
{
	for (auto& node : nodes) {
		InsertNodeRecursive(node, m_nodes);
	}
}

void Evaluator::Rename()
{
	for (auto& node : m_nodes)
	{
//		Node::PortAddr addr(node, -1);
//		int idx = 0;
		for (auto& port : node->GetImports()) {
			InsertVar(*node, port.var);
//			addr.idx = idx++;
		}
//		idx = 0;
		for (auto& port : node->GetExports()) {
			InsertVar(*node, port.var);
//			addr.idx = idx++;
		}
	}
}

void Evaluator::EvalDeclareOutside()
{
	for (auto& itr : m_vars_name2type)
	{
		if (itr.second.qualifier == VT_TEMP) {
			continue;
		}
		m_shader += cpputil::StringHelper::Format(
			"%s %s;\n", itr.second.ToGLSL(m_st).c_str(), itr.first.c_str()
		);
	}
}

void Evaluator::EvalDeclareInside(std::string& dst)
{
	for (auto& itr : m_vars_name2type)
	{
		if (itr.second.qualifier != VT_TEMP) {
			continue;
		}
		dst += cpputil::StringHelper::Format(
			"%s %s;\n", itr.second.ToGLSL(m_st).c_str(), itr.first.c_str()
		);
	}
}

void Evaluator::EvalBody()
{
	std::string body;

	EvalDeclareInside(body);

	body += "\n";
	for (auto& itr = m_nodes.rbegin(); itr != m_nodes.rend(); ++itr)
	{
		auto str = (*itr)->ToString();
		if (!str.empty())
		{
			// rename vars
			for (auto& p : (*itr)->GetImports()) {
				auto old = cpputil::StringHelper::Format("(%s)", p.var.Name().c_str());
				cpputil::StringHelper::ReplaceAll(str, old, p.var.GetRealName());
			}
			for (auto& p : (*itr)->GetExports()) {
				auto old = cpputil::StringHelper::Format("(%s)", p.var.Name().c_str());
				cpputil::StringHelper::ReplaceAll(str, old, p.var.GetRealName());
			}
			body += str +"\n";
		}
	}

	// final assign
	assert(!m_nodes.empty());
	auto& exports = m_nodes[0]->GetExports();
	assert(exports.size() == 1 && exports[0].var.Type().dim == VT_4);
	switch (m_st)
	{
	case ST_VERT:
		body += cpputil::StringHelper::Format("gl_Position = %s;\n", exports[0].var.Name().c_str());
		break;
	case ST_FRAG:
		body += cpputil::StringHelper::Format("gl_FragColor = %s;\n", exports[0].var.Name().c_str());
		break;
	}

	m_shader += cpputil::StringHelper::Format(R"(
void main()
{
%s
}
	)", body.c_str());
}

void Evaluator::InsertNodeRecursive(const sw::NodePtr& node, std::vector<sw::NodePtr>& array)
{
	array.push_back(node);
	for (auto& port : node->GetImports())
	{
		if (port.conns.empty()) {
			continue;
		}
		assert(port.conns.size() == 1);
		auto node = port.conns[0].node.lock();
		assert(node);
		InsertNodeRecursive(node, array);
	}
}

void Evaluator::InsertVar(const Node& node, Variable& var)
{
	auto name = var.GetRealName();
	auto itr = m_vars_name2type.find(name);
	if (itr == m_vars_name2type.end())
	{
		m_vars_name2type.insert({ name, var.Type() });
	}
	else
	{
		if (var.Type() != itr->second) {
			auto name = var.Name() + "_" + std::to_string(node.GetID());
			var.SetRealName(name);
			m_vars_name2type.insert({ name, var.Type() });
		}
	}
}

}
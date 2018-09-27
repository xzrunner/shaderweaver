#include "shaderweaver/Evaluator.h"
#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"
#include "shaderweaver/node/VertexShader.h"
#include "shaderweaver/node/FragmentShader.h"

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
	Concatenate();

	EvalHeader();
	EvalBody();
}

void Evaluator::InitNodes(const std::vector<NodePtr>& nodes)
{
	assert(!nodes.empty());
	auto& exports = nodes[0]->GetExports();
	assert(exports.size() >= 1);
	switch (m_st)
	{
	case ST_VERT:
	{
		auto end = std::make_shared<node::VertexShader>();
		m_nodes.push_back(end);
		sw::make_connecting({ nodes[0], 0 }, { end, 0 });
	}
		break;
	case ST_FRAG:
	{
		auto end = std::make_shared<node::FragmentShader>();
		m_nodes.push_back(end);
		sw::make_connecting({ nodes[0], 0 }, { end, 0 });
	}
		break;
	}

	std::set<sw::NodePtr> unique;
	for (auto& node : nodes) {
		InsertNodeRecursive(node, m_nodes, unique);
	}
}

void Evaluator::Rename()
{
	for (auto& node : m_nodes)
	{
		for (auto& port : node->GetExports()) {
			InsertVar(*node, port.var);
		}
		for (auto& var : node->GetInternal()) {
			InsertVar(*node, var);
		}
	}
}

void Evaluator::Concatenate()
{
	for (auto& node : m_nodes)
	{
		for (auto& port : node->GetImports())
		{
			auto in_port = port.GetPair(0);
			assert(in_port);
			Concatenate(const_cast<Node::Port&>(*in_port),
				        const_cast<Node::Port&>(port));
		}
	}
}

void Evaluator::Concatenate(Node::Port& from, Node::Port& to)
{
	auto f_type = from.var.Type();
	auto t_type = to.var.Type();

	// todo
	assert(f_type.len == t_type.len);
	assert(f_type.basis == t_type.basis);
	assert(f_type.interp == VT_INTERP_ANY
		|| t_type.interp == VT_INTERP_ANY
		|| f_type.interp == t_type.interp);
	assert(f_type.precision == VT_PRECISION_ANY
		|| t_type.precision == VT_PRECISION_ANY
		|| f_type.precision == t_type.precision);
	if (f_type.dim != t_type.dim)
	{
		std::string name = from.var.GetRealName();
		if (f_type.dim > t_type.dim)
		{
			switch (t_type.dim)
			{
			case VT_1:
				name = name + ".x";
				break;
			case VT_2:
				name = name + ".xy";
				break;
			case VT_3:
				name = name + ".xyz";
				break;
			}
		}
		else
		{
			std::string type = to.var.Type().ToGLSL();
			switch (t_type.dim)
			{
			case VT_2:
				name = cpputil::StringHelper::Format("%s(%s, %s)", type.c_str(), name.c_str(), name.c_str());
				break;
			case VT_3:
				switch (f_type.dim)
				{
				case VT_1:
					name = cpputil::StringHelper::Format("%s(%s, %s, %s)", type.c_str(), name.c_str(), name.c_str(), name.c_str());
					break;
				case VT_2:
					name = cpputil::StringHelper::Format("%s(%s, 0)", type.c_str(), name.c_str());
					break;
				}
				break;
			case VT_4:
				switch (f_type.dim)
				{
				case VT_1:
					name = cpputil::StringHelper::Format("%s(%s, %s, %s, 1)", type.c_str(), name.c_str(), name.c_str(), name.c_str());
					break;
				case VT_2:
					name = cpputil::StringHelper::Format("%s(%s, 0, 1)", type.c_str(), name.c_str());
					break;
				case VT_3:
					name = cpputil::StringHelper::Format("%s(%s, 1)", type.c_str(), name.c_str());
					break;
				}
				break;
			}
		}
		to.var.SetRealName(name);
	}
	else
	{
		to.var.SetRealName(from.var.GetRealName());
	}
}

void Evaluator::EvalHeader()
{
	std::string header;

	EvalDeclareInHeader(header);

	header += "\n";
	for (auto& itr = m_nodes.rbegin(); itr != m_nodes.rend(); ++itr)
	{
		auto str = (*itr)->GetHeaderStr();
		if (!str.empty()) {
			header += str + "\n";
		}
	}

	m_shader += header;
}

void Evaluator::EvalBody()
{
	std::string body;

	EvalDeclareInBody(body);

	body += "\n";
	for (auto& itr = m_nodes.rbegin(); itr != m_nodes.rend(); ++itr)
	{
		auto str = (*itr)->GetBodyStr();
		if (!str.empty())
		{
			// rename vars
			for (auto& p : (*itr)->GetImports()) {
				auto old = cpputil::StringHelper::Format("#%s#", p.var.Name().c_str());
				cpputil::StringHelper::ReplaceAll(str, old, p.var.GetRealName());
			}
			for (auto& p : (*itr)->GetExports()) {
				auto old = cpputil::StringHelper::Format("#%s#", p.var.Name().c_str());
				cpputil::StringHelper::ReplaceAll(str, old, p.var.GetRealName());
			}
			for (auto& v : (*itr)->GetInternal()) {
				auto old = cpputil::StringHelper::Format("#%s#", v.Name().c_str());
				cpputil::StringHelper::ReplaceAll(str, old, v.GetRealName());
			}
			body += str +"\n";
		}
	}

	m_shader += cpputil::StringHelper::Format(R"(
void main()
{
%s
}
	)", body.c_str());
}

void Evaluator::EvalDeclareInHeader(std::string& dst)
{
	for (auto& itr : m_vars_name2type)
	{
		if (itr.second.qualifier == VT_TEMP ||
			itr.second.qualifier == VT_CONST ||
			itr.second.qualifier == VT_SHADER_END) {
			continue;
		}
		dst += cpputil::StringHelper::Format(
			"%s %s;\n", itr.second.ToGLSL(m_st).c_str(), itr.first.c_str()
		);
	}
}

void Evaluator::EvalDeclareInBody(std::string& dst)
{
	for (auto& itr : m_vars_name2type)
	{
		if (itr.second.qualifier != VT_TEMP) {
			continue;
		}
		if (itr.second.region == VT_NODE_IN) {
			continue;
		}
		dst += cpputil::StringHelper::Format(
			"%s %s;\n", itr.second.ToGLSL(m_st).c_str(), itr.first.c_str()
		);
	}
}

void Evaluator::InsertNodeRecursive(const sw::NodePtr& node,
	                                std::vector<sw::NodePtr>& array,
	                                std::set<sw::NodePtr>& unique)
{
	if (unique.find(node) != unique.end()) {
		return;
	}
	unique.insert(node);
	array.push_back(node);
	for (auto& port : node->GetImports())
	{
		if (port.conns.empty()) {
			continue;
		}
		assert(port.conns.size() == 1);
		auto pair = port.conns[0].node.lock();
		assert(pair);
		InsertNodeRecursive(pair, array, unique);
	}
}

void Evaluator::InsertVar(const Node& node, const Variable& var)
{
	auto name = var.GetRealName();
	auto itr = m_vars_name2type.find(name);
	if (itr == m_vars_name2type.end())
	{
		m_vars_name2type.insert({ name, var.Type() });
	}
	else
	{
		auto t0 = var.Type();
		auto t1 = itr->second;

		// todo check const value
		if (t0.qualifier == VT_UNIF || t1.qualifier == VT_UNIF ||
			t0.qualifier == VT_CONST || t1.qualifier == VT_CONST ||
			t0.readwrite == VT_ONLY_READ || t1.readwrite == VT_ONLY_READ ||
			t0 != t1)
		{
			auto name = var.Name() + "_" + std::to_string(node.GetID());
			var.SetRealName(name);
			m_vars_name2type.insert({ name, var.Type() });
		}
	}
}

}
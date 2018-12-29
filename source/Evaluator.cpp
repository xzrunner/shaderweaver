#include "shaderweaver/Evaluator.h"

#include "shaderweaver/node/VertexShader.h"
#include "shaderweaver/node/FragmentShader.h"

#include <cpputil/StringHelper.h>

#include <assert.h>

namespace
{

const int SHADER_MAX_STR_LEN = 1024;

void rename_vars(std::string& str, const sw::Node& node)
{
	for (auto& p : node.GetImports())
	{
		auto old = cpputil::StringHelper::Format("#%s#", p.var.GetName().c_str());
		if (p.var.GetType().interp == sw::VT_FUNC) {
			assert(p.conns.size() == 1);
			auto pair = p.conns[0].node.lock();
			assert(pair);
			cpputil::StringHelper::ReplaceAll(str, old, pair->GetName());
		} else {
			cpputil::StringHelper::ReplaceAll(str, old, p.var.GetRealName());
		}
	}
	for (auto& p : node.GetExports()) {
		auto old = cpputil::StringHelper::Format("#%s#", p.var.GetName().c_str());
		cpputil::StringHelper::ReplaceAll(str, old, p.var.GetRealName());
	}
	for (auto& v : node.GetInternal()) {
		auto old = cpputil::StringHelper::Format("#%s#", v.GetName().c_str());
		cpputil::StringHelper::ReplaceAll(str, old, v.GetRealName());
	}
}

}

namespace sw
{

Evaluator::Evaluator(const std::vector<NodePtr>& nodes, ShaderType st)
	: m_st(st)
{
	InitNodes(nodes);

    for (auto& node : m_body_nodes) {
        node->InitNestingConn();
    }
    for (auto& node : m_head_nodes) {
        node.first->InitNestingConn();
    }

	Rename();
	Concatenate();
}

const std::string& Evaluator::GenShaderStr() const
{
	if (m_cached_shader.empty())
	{
		m_cached_shader += EvalHeader(std::set<NodePtr>());
		m_cached_shader += cpputil::StringHelper::Format(R"(
void main()
{
%s
}
	)", EvalBody().c_str());
	}

	return m_cached_shader;
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
		m_body_nodes.push_back(end);
		sw::make_connecting({ nodes[0], 0 }, { end, 0 });
	}
		break;
	case ST_FRAG:
	{
		auto end = std::make_shared<node::FragmentShader>();
		m_body_nodes.push_back(end);
		sw::make_connecting({ nodes[0], 0 }, { end, 0 });
	}
		break;
	}

	std::set<sw::NodePtr> body_unique, head_unique;
	for (auto& node : nodes)
	{
		if (body_unique.find(node) != body_unique.end()) {
			continue;
		}
		body_unique.insert(node);
		m_body_nodes.push_back(node);
		InsertNodeRecursive(node, body_unique, head_unique, m_body_nodes, m_head_nodes);
	}
}

void Evaluator::InsertNodeRecursive(const sw::NodePtr& node,
	                                std::set<sw::NodePtr>& body_unique,
	                                std::set<sw::NodePtr>& head_unique,
	                                std::vector<NodePtr>& body_nodes,
	                                std::vector<std::pair<NodePtr, NodePtr>>& head_nodes) const
{
	for (auto& port : node->GetImports())
	{
		if (port.conns.empty()) {
			continue;
		}

		assert(port.conns.size() == 1);
		auto pair = port.conns[0].node.lock();
		assert(pair);

		if (port.var.GetType().interp == VT_FUNC)
		{
			if (head_unique.find(pair) != head_unique.end()) {
				continue;
			}
			head_unique.insert(pair);

			auto src = pair;

			auto dst_cls = node->QueryNesting(port.var.GetName());
            auto rt_type = rttr::type::get_by_name(dst_cls);
            assert(rt_type.is_valid());
            auto rt_obj = rt_type.create();

            assert(rt_obj.is_valid()/* && rt_obj.is_type<std::shared_ptr<sw::Node>>()*/);
            auto dst = rt_obj.get_value<std::shared_ptr<sw::Node>>();

			head_nodes.push_back({ src, dst });
		}
		else
		{
			if (body_unique.find(pair) != body_unique.end()) {
				continue;
			}
			body_unique.insert(pair);
			body_nodes.push_back(pair);
		}
		InsertNodeRecursive(pair, body_unique, head_unique, body_nodes, head_nodes);
	}
}

void Evaluator::Rename()
{
	for (auto& node : m_body_nodes)
	{
		for (auto& port : node->GetExports()) {
			InsertVar(*node, port.var, m_vars_name2type);
		}
		for (auto& var : node->GetInternal()) {
			InsertVar(*node, var, m_vars_name2type);
		}
	}
}

void Evaluator::InsertVar(const Node& node, const Variable& var,
	                      std::map<std::string, VariableType>& dst)
{
	auto name = var.GetRealName();
	auto itr = dst.find(name);
	if (itr == dst.end())
	{
		dst.insert({ name, var.GetType() });
	}
	else
	{
		auto t0 = var.GetType();
		auto t1 = itr->second;

		// todo check const value
		if (t0.qualifier == VT_UNIF || t1.qualifier == VT_UNIF ||
			t0.qualifier == VT_CONST || t1.qualifier == VT_CONST ||
			t0.readwrite == VT_ONLY_READ || t1.readwrite == VT_ONLY_READ ||
			t0 != t1)
		{
			auto name = var.GetName() + "_" + std::to_string(node.GetID());
			var.SetRealName(name);
			dst.insert({ name, var.GetType() });
		}
	}
}

void Evaluator::Concatenate() const
{
	for (auto& node : m_body_nodes)
	{
		for (auto& port : node->GetImports())
		{
			auto in_port = port.GetPair(0);
			if (!in_port && port.var.GetType().interp == VT_FUNC) {
				continue;
			}
			assert(in_port);
			Concatenate(const_cast<Node::Port&>(*in_port),
				        const_cast<Node::Port&>(port));
		}
	}
}

void Evaluator::Concatenate(Node::Port& from, Node::Port& to) const
{
	auto f_type = from.var.GetType();
	auto t_type = to.var.GetType();

	// todo
	assert(f_type.len == t_type.len);
	assert(f_type.basis == t_type.basis);
	assert(f_type.interp == VT_INTERP_ANY
		|| t_type.interp == VT_INTERP_ANY
		|| f_type.interp == t_type.interp);
	assert(f_type.precision == VT_PRECISION_ANY
		|| t_type.precision == VT_PRECISION_ANY
		|| f_type.precision == VT_BOOL
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
			std::string type = to.var.GetType().ToGLSL();
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

std::string Evaluator::EvalHeader(std::set<NodePtr>& created) const
{
	std::string ret;

	// declare
	for (auto& itr : m_vars_name2type)
	{
		if (itr.second.qualifier == VT_TEMP ||
			itr.second.qualifier == VT_CONST ||
			itr.second.qualifier == VT_SHADER_END) {
			continue;
		}
		ret += cpputil::StringHelper::Format(
			"%s %s;\n", itr.second.ToGLSL(m_st).c_str(), itr.first.c_str()
		);
	}

	ret += "\n";

	for (auto& itr = m_head_nodes.rbegin(); itr != m_head_nodes.rend(); ++itr) {
		ret += EvalFunc(itr->first, itr->second, created);
	}

	ret += "\n";
	for (auto& itr = m_body_nodes.rbegin(); itr != m_body_nodes.rend(); ++itr)
	{
        if (created.find(*itr) != created.end()) {
            continue;
        }

		auto str = (*itr)->GetHeaderStr();
		if (!str.empty()) {
			rename_vars(str, **itr);
			ret += str + "\n";
		}

        created.insert(*itr);
	}

	return ret;
}

std::string Evaluator::EvalBody() const
{
	std::string ret;

	// declare
	for (auto& itr : m_vars_name2type)
	{
		if (itr.second.qualifier != VT_TEMP) {
			continue;
		}
		if (itr.second.region == VT_NODE_IN) {
			continue;
		}
		ret += cpputil::StringHelper::Format(
			"%s %s;\n", itr.second.ToGLSL(m_st).c_str(), itr.first.c_str()
		);
	}

	ret += "\n";
	for (auto& itr = m_body_nodes.rbegin(); itr != m_body_nodes.rend(); ++itr)
	{
		auto str = (*itr)->GetBodyStr();
		if (!str.empty()) {
			rename_vars(str, **itr);
			ret += str +"\n";
		}
	}

	return ret;
}

std::string Evaluator::EvalFunc(const NodePtr& src, const NodePtr& dst,
                                std::set<NodePtr>& created) const
{
    if (created.find(src) != created.end()) {
        return "";
    }

	std::string str;

	auto& params = src->GetImports();
	auto& ret = src->GetExports();
	if (ret.size() > 1) {
		return str;
	}

	// return type
	if (ret.empty()) {
		str += "void";
	} else {
		str += ret[0].var.GetType().ToGLSL();
	}
	// name
	auto func_name = src->GetName();
	str += " " + src->GetName() + "(";
	// params
	for (int i = 0, n = dst->GetImports().size(); i < n; ++i)
    {
		auto& p = params[i];
        if (p.var.GetType().interp == VT_FUNC) {
            continue;
        }
        if (i != 0) {
            str += ", ";
        }
		str += p.var.GetType().ToGLSL() + " " + p.var.GetRealName();
	}
	str += ")\n";

	// body
	str += "{\n";

	Evaluator eval({ src });
	// concatenate with real params
	for (int i = 0, n = dst->GetImports().size(); i < n; ++i) {
		eval.Concatenate(const_cast<Node::Port&>(dst->GetImports()[i]), const_cast<Node::Port&>(params[i]));
	}

	auto body_str = eval.EvalBody();
	if (!ret.empty()) {
		body_str += "return " + ret[0].var.GetRealName() + ";\n";
	}

	str += body_str;
	str += "}\n";

    // header
    str = eval.EvalHeader(created) + "\n" + str;

	return str;
}

}
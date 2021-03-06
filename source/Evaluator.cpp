#include "shaderweaver/Evaluator.h"

#include "shaderweaver/node/VertexShader.h"
#include "shaderweaver/node/FragmentShader.h"
#include "shaderweaver/node/Custom.h"
#include "shaderweaver/node/Function.h"

#include <cpputil/StringHelper.h>

#include <stack>

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

Evaluator::Evaluator(const std::vector<NodePtr>& nodes)
{
	InitNodes(nodes);

    for (auto& node : m_body_nodes) {
        node->InitNestingConn();
    }
    for (auto& node : m_func_nodes) {
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
    NodesUnique unique;
    NodesCache cache;
	for (auto& node : nodes)
	{
        auto type = node->get_type();
        if (type == rttr::type::get<node::VertexShader>()) {
            m_st = ST_VERT;
        } else if (type == rttr::type::get<node::FragmentShader>()) {
            m_st = ST_FRAG;
        }

		if (unique.body.find(node) != unique.body.end()) {
			continue;
		}
        unique.body.insert(node);
        cache.body.push_back(node);
		InsertNodeRecursive(node, unique, cache);
	}

    std::copy(cache.body.begin(), cache.body.end(), std::back_inserter(m_body_nodes));
    TopologicalSorting(m_body_nodes);
    std::copy(cache.head.begin(), cache.head.end(), std::back_inserter(m_head_nodes));
    std::copy(cache.func.begin(), cache.func.end(), std::back_inserter(m_func_nodes));
    std::copy(cache.func2.begin(), cache.func2.end(), std::back_inserter(m_func2_nodes));
}

void Evaluator::InsertNodeRecursive(const sw::NodePtr& node, NodesUnique& unique, NodesCache& cache) const
{
	for (auto& port : node->GetImports())
	{
		if (port.conns.empty()) {
			continue;
		}

		assert(port.conns.size() == 1);
		auto pair = port.conns[0].node.lock();
		assert(pair);

        // function
		if (port.var.GetType().interp == VT_FUNC)
		{
			if (unique.func.find(pair) != unique.func.end()) {
				continue;
			}
			unique.func.insert(pair);

			auto src = pair;

			auto dst_cls = node->QueryNesting(port.var.GetName());
            auto rt_type = rttr::type::get_by_name(dst_cls);
            assert(rt_type.is_valid());
            auto rt_obj = rt_type.create();

            assert(rt_obj.is_valid()/* && rt_obj.is_type<std::shared_ptr<sw::Node>>()*/);
            auto dst = rt_obj.get_value<std::shared_ptr<sw::Node>>();

			cache.func.push_back({ src, dst });
		}
        // function2
        else if (pair->get_type() == rttr::type::get<sw::node::Function>())
        {
            if (unique.func2.find(pair) == unique.func2.end()) {
                unique.func2.insert(pair);
                cache.func2.push_back(pair);
            }
            if (unique.body.find(pair) == unique.body.end()) {
                unique.body.insert(pair);
                cache.body.push_back(pair);
            }
        }
        // custom
        else if (pair->get_type() == rttr::type::get<sw::node::Custom>())
        {
            if (unique.head.find(pair) == unique.head.end()) {
                unique.head.insert(pair);
                cache.head.push_back(pair);
            }
            if (unique.body.find(pair) == unique.body.end()) {
                unique.body.insert(pair);
                cache.body.push_back(pair);
            }
        }
		else
		{
			if (unique.body.find(pair) == unique.body.end()) {
                unique.body.insert(pair);
                cache.body.push_back(pair);
            }
		}
		InsertNodeRecursive(pair, unique, cache);
	}
}

void Evaluator::TopologicalSorting(std::vector<NodePtr>& nodes) const
{
    // prepare
    std::vector<int> in_deg(nodes.size(), 0);
    std::vector<std::vector<int>> out_nodes(nodes.size());
    for (int i = 0, n = nodes.size(); i < n; ++i)
    {
        auto& node = nodes[i];
        for (auto& port : node->GetImports())
        {
            if (port.conns.empty()) {
                continue;
            }

            assert(port.conns.size() == 1);
            auto from = port.conns[0].node.lock();
            assert(from);
            for (int j = 0, m = nodes.size(); j < m; ++j) {
                if (from == nodes[j]) {
                    in_deg[i]++;
                    out_nodes[j].push_back(i);
                    break;
                }
            }
        }
    }

    // sort
    std::stack<int> st;
    std::vector<NodePtr> sorted;
    for (int i = 0, n = in_deg.size(); i < n; ++i) {
        if (in_deg[i] == 0) {
            st.push(i);
        }
    }
    while (!st.empty())
    {
        int v = st.top();
        st.pop();
        sorted.push_back(nodes[v]);
        for (auto& i : out_nodes[v]) {
            assert(in_deg[i] > 0);
            in_deg[i]--;
            if (in_deg[i] == 0) {
                st.push(i);
            }
        }
    }

    nodes = sorted;
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
			//if (!in_port && port.var.GetType().interp == VT_FUNC) {
			//	continue;
			//}
            if (!in_port) {
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
			itr.second.qualifier == VT_SHADER_END ||
            itr.second.qualifier == VT_FUNC_IN ||
            itr.second.qualifier == VT_FUNC_OUT) {
			continue;
		}
        if (itr.second.array == VT_ARRAY) {
            ret += cpputil::StringHelper::Format(
                "%s %s[%d];\n", itr.second.ToGLSL(m_st).c_str(), itr.first.c_str(), itr.second.desc
            );
        } else {
            ret += cpputil::StringHelper::Format(
                "%s %s;\n", itr.second.ToGLSL(m_st).c_str(), itr.first.c_str()
            );
        }
	}

	ret += "\n";

	for (auto& itr : m_head_nodes) {
        if (created.find(itr) == created.end()) {
            ret += (itr)->GetHeaderStr();
            ret += "\n";
            created.insert(itr);
        }
	}

    ret += "\n";

	for (auto& itr : m_func_nodes) {
		ret += EvalFunc(itr.first, itr.second, created);
	}

    ret += "\n";

    for (auto& itr : m_func2_nodes) {
        ret += EvalFunc2(itr, created);
    }

	ret += "\n";
	for (auto& itr : m_body_nodes)
	{
        if (created.find(itr) != created.end()) {
            continue;
        }

		auto str = (itr)->GetHeaderStr();
		if (!str.empty()) {
			rename_vars(str, *itr);
			ret += str + "\n";
		}

        created.insert(itr);
	}

	return ret;
}

std::string Evaluator::EvalBody() const
{
	std::string ret;

	// declare
	for (auto& itr : m_vars_name2type)
	{
		if (itr.second.qualifier != VT_TEMP &&
            itr.second.qualifier != VT_FUNC_OUT) {
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
	for (auto& itr = m_body_nodes.begin(); itr != m_body_nodes.end(); ++itr)
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
	auto& func_name = src->GetName();
	str += " " + func_name + "(";
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

std::string Evaluator::EvalFunc2(const NodePtr& node, std::set<NodePtr>& created) const
{
    if (created.find(node) != created.end()) {
        return "";
    }

	std::string str;

	auto& params = node->GetImports();
	auto& ret = node->GetExports();
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
	auto& func_name = node->GetName();
	str += " " + func_name + "(";
	// params
	for (int i = 0, n = node->GetImports().size(); i < n; ++i)
    {
		auto& p = params[i];
        if (p.var.GetType().interp == VT_FUNC) {
            continue;
        }
        if (i != 0) {
            str += ", ";
        }
		str += p.var.GetType().ToGLSL() + " " + p.var.GetName();
	}
	str += ")\n";

	// body
	str += "{\n";

    assert(node->get_type() == rttr::type::get<node::Function>());
    auto func_node = std::static_pointer_cast<node::Function>(node);
	Evaluator eval({ func_node->GetEndNodes() });
	// concatenate with real params
	for (int i = 0, n = node->GetImports().size(); i < n; ++i) {
		eval.Concatenate(const_cast<Node::Port&>(node->GetImports()[i]), const_cast<Node::Port&>(params[i]));
	}

	auto body_str = eval.EvalBody();
	if (!ret.empty()) {
		body_str += "return " + ret[0].var.GetRealName() + ";\n";
	}

	str += body_str;
	str += "}\n";

	return str;
}

}
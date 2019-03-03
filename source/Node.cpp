#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"
#include "shaderweaver/NodeIDMgr.h"
#include "shaderweaver/NodeHelper.h"

#include <assert.h>

RTTR_REGISTRATION
{

rttr::registration::class_<sw::Node::Port>("sw::Node::Port")
	.property("var", &sw::Node::Port::var)
;

rttr::registration::class_<sw::Node>("sw::Node")
	.method("GetImports", &sw::Node::GetImports)
	.method("GetExports", &sw::Node::GetExports)
    .method("QueryNesting", &sw::Node::QueryNesting)
;

}

namespace sw
{

Node::Node(const std::string& name, uint32_t version)
	: m_name(name)
	, m_version(version)
{
	m_uid = NodeIDMgr::Instance()->Gen();
}

Node::~Node()
{
	NodeIDMgr::Instance()->Return(m_uid);
}

void Node::AddNesting(const std::string& name, const std::string& cls_name)
{
	m_nesting.push_back({ name, cls_name });
}

std::string Node::QueryNesting(const std::string& name) const
{
	for (auto& pair : m_nesting) {
		if (pair.first == name) {
			return pair.second;
		}
	}
	return nullptr;
}

void Node::InitNestingConn()
{
    if (!m_inited_nest_conn && m_nest_cb) {
        m_nest_cb();
    }
    m_inited_nest_conn = true;
}

std::string Node::GetHeaderStr() const
{
	return GetHeader();
}

std::string Node::GetBodyStr() const
{
	auto body = GetBody();
	if (body.empty()) {
		return "";
	}

	std::string str;
	str += "//! START " + m_name + "\n";
	if (!m_imports.empty()) {
		str += "//! @params " + VarsToString(m_imports) + "\n";
	}
	if (!m_exports.empty()) {
		str += "//! @return " + VarsToString(m_exports) + "\n";
	}
	str += GetBody();
	str += "//! END " + m_name + "\n";
	return str;
}

void Node::InitVariables(const std::vector<Variable>& input,
	                     const std::vector<Variable>& output,
	                     const std::vector<Variable>& middle)
{
	m_imports.clear();
	m_exports.clear();
	m_internal.clear();
	for (auto& v : input)
	{
		auto type = v.GetType();
		type.region = VT_NODE_IN;
		const_cast<Variable&>(v).SetType(type);
		m_imports.push_back(v);
	}
	for (auto& v : output)
	{
		auto type = v.GetType();
		type.region = VT_NODE_OUT;
		const_cast<Variable&>(v).SetType(type);
		m_exports.push_back(v);
	}
	for (auto& v : middle)
	{
		auto type = v.GetType();
		type.region = VT_NODE_MID;
		const_cast<Variable&>(v).SetType(type);
		m_internal.push_back(v);
	}
}

std::string Node::VarsToString(const std::vector<Port>& ports)
{
	std::string str;
	for (int i = 0, n = ports.size(); i < n; ++i) {
		str += ports[i].var.GetName();
		if (i != n - 1) {
			str += ", ";
		}
	}
	return str;
}

//////////////////////////////////////////////////////////////////////////
// struct Node::Port
//////////////////////////////////////////////////////////////////////////

const Node::Port* Node::Port::GetPair(int idx) const
{
	if (idx < 0 || idx >= static_cast<int>(conns.size())) {
		return nullptr;
	}

	auto& conn = conns[idx];
	auto conn_node = conn.node.lock();
	assert(conn_node);
	auto& ports = var.GetType().region == VT_NODE_IN ? conn_node->m_exports : conn_node->m_imports;
	assert(conn.idx >= 0 && conn.idx < static_cast<int>(ports.size()));
	return &ports[conn.idx];
}

//////////////////////////////////////////////////////////////////////////

void make_connecting(const Node::PortAddr& from, const Node::PortAddr& to)
{
	{
		auto node = from.node.lock();
		assert(node);
		auto& ports = node->GetExports();
		assert(from.idx >= 0 && from.idx < static_cast<int>(ports.size()));
		const_cast<Node::Port&>(ports[from.idx]).conns.push_back(to);
	}
	{
		auto node = to.node.lock();
		assert(node);
		auto& ports = node->GetImports();
		assert(to.idx >= 0 && to.idx < static_cast<int>(ports.size())
            && ports[to.idx].conns.empty());
		const_cast<Node::Port&>(ports[to.idx]).conns.push_back(from);
	}

	NodeHelper::TypePromote(from, to);
}

}
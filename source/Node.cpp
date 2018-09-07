#include "sw/Node.h"
#include "sw/VariableType.h"
#include "sw/NodeIDMgr.h"

#include <assert.h>

namespace sw
{

Node::Node(const std::string& name)
	: m_name(name)
{
	m_uid = NodeIDMgr::Instance()->Gen();
}

Node::~Node()
{
	NodeIDMgr::Instance()->Return(m_uid);
}

std::string Node::ToString() const
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

void Node::AddVariable(const Variable& var)
{
	auto type = var.Type();
	assert(type.io != VT_IO_ANY);
	if (type.io == VT_IN) {
		m_imports.push_back(var);
	} else {
		m_exports.push_back(var);
	}
}

std::string Node::VarsToString(const std::vector<Port>& ports)
{
	std::string str;
	for (int i = 0, n = ports.size(); i < n; ++i) {
		str += ports[i].var.Name();
		if (i != n - 1) {
			str += ", ";
		}
	}
	return str;
}

void make_connecting(const Node::PortAddr& from, const Node::PortAddr& to)
{
	{
		auto node = from.node.lock();
		assert(node);
		auto& ports = node->GetExports();
		assert(from.idx >= 0 && from.idx < static_cast<int>(ports.size()));
		ports[from.idx].conns.push_back(to);
	}
	{
		auto node = to.node.lock();
		assert(node);
		auto& ports = node->GetImports();
		assert(to.idx >= 0 && to.idx < static_cast<int>(ports.size()));
		ports[to.idx].conns.push_back(from);
	}
}

}
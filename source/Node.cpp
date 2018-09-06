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
	std::string str;
	str += "//! START " + m_name + "\n";
	if (!m_params.empty()) {
		str += "//! @params " + VariablesToString(m_params) + "\n";
	}
	if (!m_return.empty()) {
		str += "//! @return " + VariablesToString(m_return) + "\n";
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
		m_params.push_back(var);
	} else {
		m_return.push_back(var);
	}
}

std::string Node::VariablesToString(const std::vector<Variable>& vars)
{
	std::string str;
	for (int i = 0, n = vars.size(); i < n; ++i) {
		str += vars[i].Name();
		if (i != n - 1) {
			str += ", ";
		}
	}
	return str;
}

}
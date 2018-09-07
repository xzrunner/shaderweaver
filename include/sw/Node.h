#pragma once

#include "sw/Variable.h"

#include <boost/noncopyable.hpp>

#include <vector>

namespace sw
{

class Node : boost::noncopyable
{
public:
	Node(const std::string& name);
	~Node();

	std::string ToString() const;

	auto& GetID() const { return m_uid; }

	auto& GetImports() { return m_imports; }
	auto& GetExports() { return m_exports; }

public:
	struct PortAddr
	{
		PortAddr(const std::shared_ptr<Node>& node, int idx)
			: node(node), idx(idx) {}
		PortAddr(const PortAddr& addr)
			: node(addr.node), idx(addr.idx) {}

		std::weak_ptr<Node> node;
		int idx = -1;
	};

	struct Port
	{
		Port(const Variable& var)
			: var(var) {}

		Variable var;
		std::vector<PortAddr> conns;
	};

protected:
	virtual std::string GetBody() const = 0;

	void AddVariable(const Variable& var);

private:
	static std::string VarsToString(const std::vector<Port>& ports);

private:
	uint32_t m_uid = 0;

	std::string m_name;

	std::vector<Port> m_imports, m_exports;

}; // Node

void make_connecting(const Node::PortAddr& from, const Node::PortAddr& to);

}
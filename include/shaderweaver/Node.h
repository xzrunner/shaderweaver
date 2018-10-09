#pragma once

#include "shaderweaver/Variable.h"

#include <boost/noncopyable.hpp>

#include <vector>
#include <memory>

namespace sw
{

class Node : boost::noncopyable
{
public:
	Node(const std::string& name, uint32_t version = 110);
	~Node();

	std::string GetHeaderStr() const;
	std::string GetBodyStr() const;

	auto& GetID() const { return m_uid; }

	auto& GetImports() const { return m_imports; }
	auto& GetExports() const { return m_exports; }
	auto& GetInternal() const { return m_internal; }

	auto& GetDimGroup() const { return m_dim_group; }

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

		const Port* GetPair(int idx) const;

		Variable var;
		std::vector<PortAddr> conns;
	};

protected:
	virtual std::string GetHeader() const { return ""; }
	virtual std::string GetBody() const = 0;

	void InitVariables(const std::vector<Variable>& input, const std::vector<Variable>& output,
		const std::vector<Variable>& middle);

private:
	static std::string VarsToString(const std::vector<Port>& ports);

protected:
	std::vector<Port> m_imports, m_exports;
	std::vector<Variable> m_internal;

public:
	static const uint32_t MAX_IMPORTS_COUNT = 1024;
protected:
	std::vector<uint32_t> m_dim_group;

private:
	uint32_t m_uid = 0;

	std::string m_name;

	uint32_t m_version = 110;

}; // Node

void make_connecting(const Node::PortAddr& from, const Node::PortAddr& to);

}
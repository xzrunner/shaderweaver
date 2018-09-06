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

	auto& GetParams() const { return m_params; }
	auto& GetReturn() const { return m_return; }

	auto& GetInput() const { return m_input; }
	auto& GetOutput() const { return m_output; }

protected:
	virtual std::string GetBody() const = 0;

	void AddVariable(const Variable& var);

private:
	static std::string VariablesToString(const std::vector<Variable>& vars);

private:
	uint32_t m_uid = 0;

	std::string m_name;

	std::vector<Variable> m_params, m_return;

	std::vector<std::shared_ptr<Node>> m_input, m_output;

}; // Node

}
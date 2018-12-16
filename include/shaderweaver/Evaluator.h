#pragma once

#include "shaderweaver/typedef.h"
#include "shaderweaver/ShaderType.h"
#include "shaderweaver/Variable.h"
#include "shaderweaver/Node.h"

#include <vector>
#include <map>
#include <set>

namespace sw
{

class Evaluator
{
public:
	Evaluator(const std::vector<NodePtr>& nodes,
		ShaderType st = ST_NONE);

	const std::string& GenShaderStr() const;

	template<typename T>
	bool HasNodeType() const
	{
		for (auto& n : m_head_nodes) {
			if (std::dynamic_pointer_cast<T>(n.first)) {
				return true;
			}
		}
		for (auto& n : m_body_nodes) {
			if (std::dynamic_pointer_cast<T>(n)) {
				return true;
			}
		}
		return false;
	}

private:
	// prepare

	// 1. topologically sort nodes
	void InitNodes(const std::vector<NodePtr>& nodes);
	void InsertNodeRecursive(const sw::NodePtr& node, std::set<sw::NodePtr>& body_unique,
		std::set<sw::NodePtr>& head_unique, std::vector<NodePtr>& body_nodes,
		std::vector<std::pair<NodePtr, NodePtr>>& head_nodes) const;
	// 2. pepare vars
	void Rename();
	static void InsertVar(const Node& node, const Variable& var,
		std::map<std::string, VariableType>& dst);
	// 3. connect, type conv
	void Concatenate() const;
	void Concatenate(Node::Port& from, Node::Port& to) const;

	// gen
	std::string EvalHeader() const;
	std::string EvalBody() const;

	std::string EvalFunc(const NodePtr& src, const NodePtr& dst) const;

private:
	ShaderType m_st;

	std::vector<NodePtr> m_body_nodes;
	std::vector<std::pair<NodePtr, NodePtr>> m_head_nodes;

	std::map<std::string, VariableType> m_vars_name2type;

	mutable std::string m_cached_shader;

}; // Evaluator

}
#pragma once

#include "shaderweaver/typedef.h"
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
	Evaluator(const std::vector<NodePtr>& nodes);

	const std::string& GenShaderStr() const;

	template<typename T>
	bool HasNodeType() const
	{
		for (auto& n : m_func_nodes) {
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
    struct NodesUnique {
        std::set<sw::NodePtr> body, head, func, func2;
    };
    struct NodesCache {
        std::vector<NodePtr> body, head, func2;
        std::vector<std::pair<NodePtr, NodePtr>> func;
    };
	void InitNodes(const std::vector<NodePtr>& nodes);
	void InsertNodeRecursive(const sw::NodePtr& node, NodesUnique& unique, NodesCache& cache) const;
    void TopologicalSorting(std::vector<NodePtr>& nodes) const;
	// 2. pepare vars
	void Rename();
	static void InsertVar(const Node& node, const Variable& var,
		std::map<std::string, VariableType>& dst);
	// 3. connect, type conv
	void Concatenate() const;
	void Concatenate(Node::Port& from, Node::Port& to) const;

	// gen
	std::string EvalHeader(std::set<NodePtr>& created) const;
	std::string EvalBody() const;

	std::string EvalFunc(const NodePtr& src, const NodePtr& dst,
        std::set<NodePtr>& created) const;
    std::string EvalFunc2(const NodePtr& node, std::set<NodePtr>& created) const;

private:
	ShaderType m_st = ST_NONE;

	std::vector<NodePtr> m_body_nodes;
    std::vector<NodePtr> m_head_nodes;
	std::vector<std::pair<NodePtr, NodePtr>> m_func_nodes;
    std::vector<NodePtr> m_func2_nodes;

	std::map<std::string, VariableType> m_vars_name2type;

	mutable std::string m_cached_shader;

}; // Evaluator

}
#pragma once

#include "shaderweaver/Variable.h"
#include "shaderweaver/typedef.h"
#include "shaderweaver/Node.h"

#include <cstdint>
#include <vector>
#include <map>
#include <set>

namespace sw
{

class Evaluator
{
public:
	Evaluator(const std::vector<NodePtr>& nodes, ShaderType st);

	auto& GetShaderStr() const { return m_shader; }

	template<typename T>
	bool HasNodeType() const {
		for (auto& n : m_nodes) {
			if (std::dynamic_pointer_cast<T>(n)) {
				return true;
			}
		}
		return false;
	}

private:
	// topologically sort
	void InitNodes(const std::vector<NodePtr>& nodes);

	void Rename();

	void Concatenate();
	void Concatenate(Node::Port& from, Node::Port& to);

	void EvalHeader();
	void EvalBody();

	void EvalDeclareInHeader(std::string& dst);
	void EvalDeclareInBody(std::string& dst);

private:
	static void InsertNodeRecursive(const sw::NodePtr& node,
		std::vector<sw::NodePtr>& array, std::set<sw::NodePtr>& unique);

	void InsertVar(const Node& node, const Variable& var);

private:
	ShaderType m_st;

	std::string m_shader;

	std::vector<NodePtr> m_nodes;

	std::map<std::string, VariableType> m_vars_name2type;

}; // Evaluator

}
#pragma once

#include "sw/Variable.h"
#include "sw/typedef.h"
#include "sw/Node.h"

#include <cstdint>
#include <vector>
#include <map>

namespace sw
{

class Evaluator
{
public:
	Evaluator(const std::vector<NodePtr>& nodes, ShaderType st);

	auto& GetShaderStr() const { return m_shader; }

private:
	// topologically sort
	void InitNodes(const std::vector<NodePtr>& nodes);

	void Rename();

	void EvalDeclareOutside();
	void EvalDeclareInside(std::string& dst);
	void EvalBody();

private:
	static void InsertNodeRecursive(const sw::NodePtr& node, std::vector<sw::NodePtr>& array);

	void InsertVar(const Node& node, Variable& var);

private:
	ShaderType m_st;

	std::string m_shader;

	std::vector<NodePtr> m_nodes;

	std::map<std::string, VariableType> m_vars_name2type;

}; // Evaluator

}
#pragma once

#include "sw/Variable.h"
#include "sw/typedef.h"

#include <cstdint>
#include <vector>
#include <map>

namespace sw
{

class Evaluator
{
public:
	Evaluator(const NodePtr& vert, const NodePtr& frag);

	auto& GetVert() const { return m_vert; }
	auto& GetFrag() const { return m_frag; }

private:
	struct Flatten
	{
		Flatten(const NodePtr& node);

		std::vector<std::pair<Variable, uint32_t>> vars;

		std::vector<NodePtr>                       nodes;
		std::map<uint32_t, size_t>                 node2vars;

		void Insert(const NodePtr& node);

	}; // Flatten

	class VariablesRename
	{
	public:
		VariablesRename(const Flatten& ft);

		auto& GetRenamedVars() const { return m_dst; }

	private:
		void Rename();

	private:
		const Flatten&        m_src;

		std::vector<int>      m_src2dst;
		std::vector<Variable> m_dst;

	}; // VariablesRename

	class ConcatenateNodes
	{
	public:


	}; // ConcatenateNodes

private:
	static void EvalDeclareOutside(std::string& dst, const VariablesRename& src, bool is_vert);
	static void EvalDeclareInside(std::string& dst, const VariablesRename& src);
	static void EvalBody(std::string& dst, const VariablesRename& src_names,
		const Flatten& src, bool is_vert);

private:
	std::string m_vert, m_frag;

}; // Evaluator

}
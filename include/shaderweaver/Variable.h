#pragma once

#include "shaderweaver/VariableType.h"

#include <string>

namespace sw
{

class Variable
{
public:
	Variable();
	Variable(VariableType type, const std::string& name);
	Variable(uint32_t type, const std::string& name);

	auto& GetType() const { return m_type; }
	void  SetType(const VariableType& type) { m_type = type; }
	auto& GetName() const { return m_name; }
	void  SetName(const std::string& name) { m_name = name; }

	void  SetRealName(const std::string& name) const { m_real_name = name; }
	auto& GetRealName() const { return m_real_name; }

	bool IsDimDynamic() const { return m_is_dim_dynamic; }

private:
	bool m_is_dim_dynamic = false;
	VariableType m_type;
	std::string  m_name;
	mutable std::string m_real_name;

}; // Variable

}
#pragma once

#include "shaderweaver/VariableType.h"

#include <string>

namespace sw
{

class Variable
{
public:
	Variable(VariableType type, const std::string& name);
	Variable(uint32_t type, const std::string& name);

	auto& Type() { return m_type; }
	auto& Type() const { return m_type; }
	auto& Name() const { return m_name; }

	void  SetRealName(const std::string& name) const { m_real_name = name; }
	auto& GetRealName() const { return m_real_name; }

	void SetType(VariableType type) { m_type = type; }

private:
	VariableType m_type;
	std::string  m_name;
	mutable std::string m_real_name;

}; // Variable

}
#pragma once

#include "sw/VariableType.h"

#include <string>

namespace sw
{

class Variable
{
public:
	Variable(VariableType type, const std::string& name);
	Variable(uint32_t type, const std::string& name);

	auto& Type() const { return m_type; }
	auto& Name() const { return m_name; }

private:
	void FixName();

private:
	VariableType m_type;
	std::string  m_name;

}; // Variable

}
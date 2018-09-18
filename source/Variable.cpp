#include "shaderweaver/Variable.h"

namespace sw
{

Variable::Variable(VariableType type, const std::string& name)
	: m_type(type), m_name(name), m_real_name(name)
{
}

Variable::Variable(uint32_t type, const std::string& name)
	: m_name(name), m_real_name(name)
{
	m_type.u32 = type;
}

}
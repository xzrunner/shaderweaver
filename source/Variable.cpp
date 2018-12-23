#include "shaderweaver/Variable.h"

#include <rttr/registration.h>

RTTR_REGISTRATION
{

rttr::registration::class_<sw::Variable>("sw::Variable")
	.property("type", &sw::Variable::GetType, &sw::Variable::SetType)
	.property("name", &sw::Variable::GetName, &sw::Variable::SetName)
;

}

namespace sw
{

Variable::Variable()
{
	m_is_dim_dynamic = false;
	m_type.u32 = 0;
}

Variable::Variable(VariableType type, const std::string& name, bool default_input)
	: m_is_dim_dynamic(type.dim)
	, m_type(type)
	, m_name(name)
	, m_real_name(name)
	, m_default_input(default_input)
{
}

Variable::Variable(uint32_t type, const std::string& name, bool default_input)
	: m_name(name)
	, m_real_name(name)
	, m_default_input(default_input)
{
	m_type.u32 = type;

	m_is_dim_dynamic = m_type.dim == 0;
}

}
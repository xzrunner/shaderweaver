#include "sw/Variable.h"

namespace sw
{

Variable::Variable(VariableType type, const std::string& name)
	: m_type(type), m_name(name)
{
	FixName();
}

Variable::Variable(uint32_t type, const std::string& name)
	: m_name(name)
{
	m_type.u32 = type;
	FixName();
}

void Variable::FixName()
{
	switch (m_type.qualifier)
	{
	case VT_VARYING:
		m_name.insert(0, "v_");
		break;
	case VT_UNIFORM:
		m_name.insert(0, "u_");
		break;
	}
}

}
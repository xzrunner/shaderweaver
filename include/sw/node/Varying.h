#pragma once

#include "sw/Node.h"
#include "sw/VariableType.h"

#include <cpputil/StringHelper.h>

namespace sw
{
namespace node
{

class Varying : public Node
{
public:
	Varying(const std::string& name, uint32_t type)
		: Node("Varying")
		, m_name(name)
	{
		AddVariable(Variable(type | t_attr | t_in, name));
		AddVariable(Variable(type | t_vary | t_out, name));
	}

protected:
	virtual std::string GetBody() const override
	{
		// v_name = name;
		return cpputil::StringHelper::Format(
			"v_%s = %s;\n", m_name.c_str(), m_name.c_str()
		);
	}

private:
	std::string m_name;

}; // Varying

}
}
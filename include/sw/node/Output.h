#pragma once

#include "sw/Node.h"
#include "sw/VariableType.h"

#include <cpputil/StringHelper.h>

namespace sw
{
namespace node
{

class Output : public sw::Node
{
public:
	Output(const std::string& name, uint32_t type)
		: Node("Output")
		, m_var_name(name)
	{
		AddVariable(Variable(t_n_in | type, name));
		AddVariable(Variable(t_n_out | t_s_out | type, name));
	}

protected:
	virtual std::string GetBody() const override
	{
		return cpputil::StringHelper::Format(
			"%s = (%s);\n", m_var_name.c_str(), m_var_name.c_str()
		);
	}

private:
	std::string m_var_name;

}; // Output

}
}
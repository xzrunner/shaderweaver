#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

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
		InitVariables({
			{ type, name },
		}, {
			{ t_s_out | type, name },
		}, {
		});
	}

protected:
	virtual std::string GetBody() const override
	{
		return cpputil::StringHelper::Format(
			"%s = #%s#;\n", m_var_name.c_str(), m_var_name.c_str()
		);
	}

private:
	std::string m_var_name;

	RTTR_ENABLE(Node)

}; // Output

}
}
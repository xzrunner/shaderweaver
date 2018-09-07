#pragma once

#include "sw/Node.h"
#include "sw/VariableType.h"

#include <cpputil/StringHelper.h>

namespace sw
{
namespace node
{

class Output : public Node
{
public:
	Output(const std::string& name, uint32_t type)
		: Node("Output")
	{
		AddVariable(Variable(t_in | t_sd_out | type, name));
	}

protected:
	virtual std::string GetBody() const override
	{

		//return cpputil::StringHelper::Format(
		//	"%s = %s;\n", );

		return "";
	}

}; // Output

}
}
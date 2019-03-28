#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

#include <cpputil/StringHelper.h>

namespace sw
{
namespace node
{

class PI : public sw::Node
{
public:
	PI()
		: Node("PI")
	{
		InitVariables({
			{ t_flt1, "multiplier" },
		}, {
			{ t_flt1, "_out" },
		}, {
		});
	}

protected:
	virtual std::string GetBody() const override
	{
        return cpputil::StringHelper::Format("#_out# = #multiplier# * 3.14159265359;\n");
	}

	RTTR_ENABLE(Node)

}; // PI

}
}
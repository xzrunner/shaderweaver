#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class FresnelSchlick : public sw::Node
{
public:
	FresnelSchlick()
		: Node("FresnelSchlick")
	{
		InitVariables({
			{ t_flt1, "cos_theta" },
            { t_flt3, "F0" },
		}, {
			{ t_col3, "_out" },
		}, {
		});
	}

protected:
	virtual std::string GetBody() const override
	{
		return R"(
#_out# = #F0# + (1.0 - #F0#) * pow(1.0 - #cos_theta#, 5.0);
)" + 1;
	}

	RTTR_ENABLE(Node)

}; // FresnelSchlick

}
}
#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class FresnelSchlickRoughness : public sw::Node
{
public:
	FresnelSchlickRoughness()
		: Node("FresnelSchlickRoughness")
	{
		InitVariables({
			{ t_flt1, "cos_theta" },
            { t_flt3, "F0" },
            { t_flt1, "roughness" },
		}, {
			{ t_col3, "_out" },
		}, {
		});
	}

protected:
	virtual std::string GetBody() const override
	{
		return R"(
#_out# = #F0# + (max(vec3(1.0 - #roughness#), #F0#) - #F0#) * pow(1.0 - #cos_theta#, 5.0);
)" + 1;
	}

	RTTR_ENABLE(Node)

}; // FresnelSchlickRoughness

}
}
// code from https://www.shadertoy.com/view/lt33z7

#pragma once

#include "shaderweaver/node/SDF.h"

namespace sw
{
namespace node
{

class EstimateNormal : public Node
{
public:
	EstimateNormal()
		: Node("EstimateNormal")
	{
		InitVariables({
			{ t_flt3, "pos" },
			{ t_func, "sdf" },
		}, {
			{ t_flt3, "normal" },
		}, {
		});

		AddNesting("sdf", std::make_shared<SDF>());
	}

protected:
	virtual std::string GetHeader() const override
	{
		return "const float EPSILON = 0.0001;\n";
	}

	virtual std::string GetBody() const override
	{
		return R"(
#normal# = normalize(vec3(
    #sdf#(vec3(p.x + EPSILON, p.y, p.z))  - #sdf#(vec3(p.x - EPSILON, p.y, p.z)),
    #sdf#(vec3(p.x, p.y + EPSILON, p.z))  - #sdf#(vec3(p.x, p.y - EPSILON, p.z)),
    #sdf#(vec3(p.x, p.y, p.z  + EPSILON)) - #sdf#(vec3(p.x, p.y, p.z - EPSILON))
));
)" + 1;
	}

	RTTR_ENABLE(Node)

}; // EstimateNormal

}
}
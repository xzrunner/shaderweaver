// code from https://www.shadertoy.com/view/lt33z7

#pragma once

#include "shaderweaver/Node.h"

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
			{ t_func, "sdf", false },
		}, {
			{ t_flt3, "normal" },
		}, {
		});

        AddNesting("sdf", "sw::SDF");
	}

	enum InputID
	{
		ID_POS = 0,
		ID_SDF,
	};

protected:
	virtual std::string GetHeader() const override
	{
		return "const float EPSILON_NORM = 0.0001;\n";
	}

	virtual std::string GetBody() const override
	{
		return R"(
#normal# = normalize(vec3(
    #sdf#(vec3(#pos#.x + EPSILON_NORM, #pos#.y, #pos#.z))  - #sdf#(vec3(#pos#.x - EPSILON_NORM, #pos#.y, #pos#.z)),
    #sdf#(vec3(#pos#.x, #pos#.y + EPSILON_NORM, #pos#.z))  - #sdf#(vec3(#pos#.x, #pos#.y - EPSILON_NORM, #pos#.z)),
    #sdf#(vec3(#pos#.x, #pos#.y, #pos#.z  + EPSILON_NORM)) - #sdf#(vec3(#pos#.x, #pos#.y, #pos#.z - EPSILON_NORM))
));
)" + 1;
	}

	RTTR_ENABLE(Node)

}; // EstimateNormal

}
}
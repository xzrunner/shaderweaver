#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"

namespace sw
{
namespace node
{

class NormalBlend : public sw::Node
{
public:
	NormalBlend()
		: Node("NormalBlend")
	{
		InitVariables({
			{ t_flt3, "a" },
			{ t_flt3, "b" },
		}, {
			{ t_flt3, "_out" },
		}, {
		});
	}

	enum InputID
	{
		ID_A = 0,
		ID_B,
	};

protected:
	virtual std::string GetBody() const override
	{
		return "#_out# = normalize(vec3(#a#.rg + #b#.rg, #a#.b * #b#.b));\n";
	}

}; // NormalBlend

}
}
#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"

namespace sw
{
namespace node
{

class NormalStrength : public sw::Node
{
public:
	NormalStrength()
		: Node("NormalStrength")
	{
		InitVariables({
			{ t_flt3, "_in" },
			{ t_flt1, "strength" },
		}, {
			{ t_flt3, "_out" },
		}, {
		});
	}

	enum InputID
	{
		ID_INPUT = 0,
		ID_STRENGTH,
	};

protected:
	virtual std::string GetBody() const override
	{
		return "#_out# = vec3(#_in#.rg * #strength#, #_in#.b);\n";
	}

}; // NormalStrength

}
}
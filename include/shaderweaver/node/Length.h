#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"

namespace sw
{
namespace node
{

class Length : public sw::Node
{
public:
	Length()
		: Node("Length")
	{
		InitVariables({
			{ 0, "_in" },
		}, {
			{ t_flt1, "_out" },
		}, {
		});
	}

protected:
	virtual std::string GetBody() const override
	{
		return "#_out# = length(#_in#);\n";
	}

	RTTR_ENABLE(Node)

}; // Length

}
}
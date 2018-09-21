#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"
#include "shaderweaver/VariableType.h"

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
			{ 0, "len_in" },
		}, {
			{ t_flt1, "len_out" },
		}, {
		});
	}

protected:
	virtual std::string GetBody() const override
	{
		return "(len_out) = length((len_in));\n";
	}

}; // Length

}
}
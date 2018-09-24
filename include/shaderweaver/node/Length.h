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
			{ 0, "a" },
		}, {
			{ t_flt1, "len" },
		}, {
		});
	}

protected:
	virtual std::string GetBody() const override
	{
		return "(len) = length((a));\n";
	}

}; // Length

}
}
#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

#include <cpputil/StringHelper.h>

namespace sw
{
namespace node
{

class VertexTangent : public sw::Node
{
public:
	VertexTangent()
		: Node("VertexTangent")
	{
		InitVariables({
		}, {
			{ t_flt3, "xyz" },
            { t_flt1, "x" },
            { t_flt1, "y" },
            { t_flt1, "z" },
		}, {
		});
	}

protected:
	virtual std::string GetBody() const override
	{
        // todo
        return "";
	}

	RTTR_ENABLE(Node)

}; // VertexTangent

}
}
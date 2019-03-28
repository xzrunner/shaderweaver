#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class WorldBitangent : public sw::Node
{
public:
	WorldBitangent()
		: Node("WorldBitangent")
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

}; // WorldBitangent

}
}
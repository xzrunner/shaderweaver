#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class WorldPosition : public sw::Node
{
public:
	WorldPosition()
		: Node("WorldPosition")
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

}; // WorldPosition

}
}
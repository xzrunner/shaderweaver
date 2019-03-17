#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class WorldNormalVector : public sw::Node
{
public:
	WorldNormalVector()
		: Node("WorldNormalVector")
	{
		InitVariables({
			{ t_flt3, "normal" },
		}, {
			{ t_flt3, "xyz" },
            { t_flt1, "x" },
            { t_flt1, "y" },
            { t_flt1, "z" },
		}, {
		});
	}

    RTTR_ENABLE(Node)

}; // WorldNormalVector

}
}
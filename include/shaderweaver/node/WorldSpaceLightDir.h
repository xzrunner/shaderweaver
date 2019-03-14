#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class WorldSpaceLightDir : public Node
{
public:
    WorldSpaceLightDir()
        : Node("WorldSpaceLightDir")
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

    RTTR_ENABLE(Node)

}; // WorldSpaceLightDir

}
}
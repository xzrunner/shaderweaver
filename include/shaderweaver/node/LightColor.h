#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class LightColor : public Node
{
public:
    LightColor()
        : Node("LightColor")
    {
		InitVariables({
		}, {
            { t_col3, "rgba" },
            { t_flt3, "color" },
            { t_flt1, "intensity" },
		}, {
		});
    }

    RTTR_ENABLE(Node)

}; // LightColor

}
}
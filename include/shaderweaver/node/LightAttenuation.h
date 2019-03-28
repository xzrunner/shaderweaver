#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class LightAttenuation : public Node
{
public:
    LightAttenuation()
        : Node("LightAttenuation")
    {
		InitVariables({
		}, {
            { t_flt1, "_out" },
		}, {
		});
    }

    RTTR_ENABLE(Node)

}; // LightAttenuation

}
}
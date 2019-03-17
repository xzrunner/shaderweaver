#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class IndirectSpecularLight : public Node
{
public:
    IndirectSpecularLight()
        : Node("IndirectSpecularLight")
    {
		InitVariables({
            { t_flt3, "normal" },
            { t_flt1, "roughness" },
            { t_flt1, "occlusion" },
		}, {
            { t_flt3, "rgb" },
		}, {
		});
    }

    RTTR_ENABLE(Node)

}; // IndirectSpecularLight

}
}
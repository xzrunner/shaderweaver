#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class IndirectDiffuseLighting : public Node
{
public:
    IndirectDiffuseLighting()
        : Node("IndirectDiffuseLighting")
    {
		InitVariables({
            { t_flt3, "normal" },
		}, {
            { t_flt3, "rgb" },
		}, {
		});
    }

    RTTR_ENABLE(Node)

}; // IndirectDiffuseLighting

}
}
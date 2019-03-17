#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class TextureTransform : public Node
{
public:
	TextureTransform()
		: Node("TextureTransform")
	{
		InitVariables({
            { t_tex2d, "tex" },
		}, {
            { t_flt2,  "tilling" },
            { t_flt2,  "offset" },
        }, {
		});
	}

	RTTR_ENABLE(Node)

}; // TextureTransform

}
}
#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class TexCoords : public Node
{
public:
	TexCoords()
		: Node("TexCoords")
	{
		InitVariables({
            { t_tex2d, "tex" },
            { t_flt2,  "tilling" },
            { t_flt2,  "offset" },
		}, {
            { t_uv,    "uv" },
            { t_flt1,  "u" },
            { t_flt1,  "v" },
		}, {
		});
	}

	RTTR_ENABLE(Node)

}; // TexCoords

}
}
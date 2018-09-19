#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class SampleTex2D : public sw::Node
{
public:
	SampleTex2D()
		: Node("SampleTex2D")
	{
		InitVariables({
			{ t_tex2d, "tex" },
			{ t_uv, "uv" },
		}, {
			{ t_col4 | t_read, "col4" },
		}, {
		});
	}

	enum InputID
	{
		ID_TEX = 0,
		ID_UV,
	};

protected:
	virtual std::string GetBody() const override
	{
		// todo: in phong, result will be changed by another one before using
		// 1. clip phong to small block, or
		// 2. add special flag to make result read only
		return "(col4) = texture2D((tex), (uv));\n";
	}

}; // SampleTex2D

}
}
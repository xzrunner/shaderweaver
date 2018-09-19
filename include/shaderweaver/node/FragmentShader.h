#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class FragmentShader : public Node
{
public:
	FragmentShader()
		: Node("FragmentShader")
	{
		InitVariables({
			// use t_flt4 to accept uv
			{ /*t_col4*/t_flt4, "col" },
		}, {
			{ t_col4 | t_s_end, "gl_FragColor" },
		}, {
		});
	}

protected:
	virtual std::string GetBody() const override
	{
		return "(gl_FragColor) = (col);\n";
	}

}; // FragmentShader

}
}
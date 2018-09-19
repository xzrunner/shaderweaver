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
		// use t_flt4 to accept uv
		AddVariable(Variable(/*t_col4*/t_flt4 | t_n_in, "col"));
		AddVariable(Variable(t_col4 | t_n_out | t_s_end, "gl_FragColor"));
	}

protected:
	virtual std::string GetBody() const override
	{
		return "(gl_FragColor) = (col);\n";
	}

}; // FragmentShader

}
}
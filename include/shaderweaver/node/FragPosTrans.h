#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class FragPosTrans : public Node
{
public:
	FragPosTrans()
		: Node("FragPosTrans")
	{
		InitVariables({
			{ t_mat4, "model" },
			{ t_pos4, "pos" },
		}, {
			{ t_flt3 | t_s_out, "frag_pos" },
		}, {
		});
	}

	enum ImportIdx
	{
		IN_MODEL = 0,
		IN_POS,
	};

protected:
	virtual std::string GetBody() const override
	{
		return "(frag_pos) = vec3((model) * (pos));\n";
	}

}; // FragPosTrans

}
}
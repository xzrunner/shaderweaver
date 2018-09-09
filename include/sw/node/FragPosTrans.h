#pragma once

#include "sw/Node.h"
#include "sw/VariableType.h"

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
		AddVariable(Variable(t_mat4 | t_n_in, "model"));
		AddVariable(Variable(t_pos4 | t_n_in, "pos"));

		AddVariable(Variable(t_flt3 | t_n_out | t_s_out, "frag_pos"));
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
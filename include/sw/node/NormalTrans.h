#pragma once

#include "sw/Node.h"
#include "sw/VariableType.h"

namespace sw
{
namespace node
{

class NormalTrans : public Node
{
public:
	NormalTrans()
		: Node("NormalTrans")
	{
		AddVariable(Variable(t_mat4 | t_n_in, "model"));
		AddVariable(Variable(t_nor3 | t_n_in, "nor"));

		AddVariable(Variable(t_nor3 | t_n_out, "nor_trans"));

	}

	enum ImportIdx
	{
		IN_MODEL = 0,
		IN_NORM,
	};

protected:
	virtual std::string GetBody() const override
	{
		// todo: no transpose and inverse
//		return "(nor_trans) = mat3(transpose(inverse((model)))) * (nor);\n";
		return "(nor_trans) = mat3((model)) * (nor);\n";
	}

}; // NormalTrans

}
}
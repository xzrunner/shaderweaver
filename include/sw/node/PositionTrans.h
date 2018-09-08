#pragma once

#include "sw/Node.h"
#include "sw/VariableType.h"

namespace sw
{
namespace node
{

class PositionTrans : public Node
{
public:
	PositionTrans()
		: Node("PositionTrans")
	{
		AddVariable(Variable(t_mat4 | t_n_in, "proj"));
		AddVariable(Variable(t_mat4 | t_n_in, "modelview"));
		AddVariable(Variable(t_pos4 | t_n_in, "pos"));

		AddVariable(Variable(t_pos4 | t_n_out, "pos_trans"));
	}

	enum ImportIdx
	{
		IN_PROJ = 0,
		IN_MODELVIEW,
		IN_POS,
	};

protected:
	virtual std::string GetBody() const override
	{
		return "(pos_trans) = (proj) * (modelview) * (pos);\n";
	}

}; // PositionTrans

}
}
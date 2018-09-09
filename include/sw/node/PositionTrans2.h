#pragma once

#include "sw/Node.h"
#include "sw/VariableType.h"

namespace sw
{
namespace node
{

class PositionTrans2 : public Node
{
public:
	PositionTrans2()
		: Node("PositionTrans2")
	{
		AddVariable(Variable(t_mat4 | t_n_in, "proj"));
		AddVariable(Variable(t_mat4 | t_n_in, "view"));
		AddVariable(Variable(t_mat4 | t_n_in, "model"));
		AddVariable(Variable(t_pos4 | t_n_in, "pos"));

		AddVariable(Variable(t_pos4 | t_n_out, "pos_trans"));
	}

	enum ImportIdx
	{
		IN_PROJ = 0,
		IN_VIEW,
		IN_MODEL,
		IN_POS,
	};

protected:
	virtual std::string GetBody() const override
	{
		return "(pos_trans) = (proj) * (view) * (model) * (pos);\n";
	}

}; // PositionTrans2

}
}
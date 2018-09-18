#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

#include <assert.h>

namespace sw
{
namespace node
{

class PositionTransOld : public sw::Node
{
public:
	PositionTransOld(int dim)
		: Node("PositionTransOld")
		, m_dim(dim)
	{
		AddVariable(Variable(t_mat4 | t_n_in, "proj"));
		AddVariable(Variable(t_mat4 | t_n_in, "modelview"));
		switch (dim) {
		case 2:
			AddVariable(Variable(t_pos2 | t_n_in, "pos"));
			break;
		case 3:
			AddVariable(Variable(t_pos3 | t_n_in, "pos"));
			break;
		case 4:
			AddVariable(Variable(t_pos4 | t_n_in, "pos"));
			break;
		default:
			assert(0);
		}

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
		switch (m_dim)
		{
		case 2:
			return "(pos_trans) = (proj) * (modelview) * vec4((pos), 0.0, 1.0);\n";
		case 3:
			return "(pos_trans) = (proj) * (modelview) * vec4((pos), 1.0);\n";
		case 4:
			return "(pos_trans) = (proj) * (modelview) * (pos);\n";
		default:
			return "";
		}
	}

private:
	int m_dim;

}; // PositionTransOld

}
}
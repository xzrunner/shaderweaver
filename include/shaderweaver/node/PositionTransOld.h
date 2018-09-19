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
		uint32_t pos_type;
		switch (dim) {
		case 2:
			pos_type = t_pos2;
			break;
		case 3:
			pos_type = t_pos3;
			break;
		case 4:
			pos_type = t_pos4;
			break;
		default:
			assert(0);
		}

		InitVariables({
			{ t_mat4, "proj" },
			{ t_mat4, "modelview" },
			{ pos_type, "pos" },
		}, {
			{ t_pos4, "pos_trans" },
		}, {
		});
	}

	enum InputID
	{
		ID_PROJ = 0,
		ID_MODELVIEW,
		ID_POS,
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
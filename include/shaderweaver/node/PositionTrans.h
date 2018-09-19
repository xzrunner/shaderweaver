#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

#include <assert.h>

namespace sw
{
namespace node
{

class PositionTrans : public Node
{
public:
	PositionTrans(int dim)
		: Node("PositionTrans")
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
			{ t_mat4, "view" },
			{ t_mat4, "model" },
			{ pos_type, "pos" },
		}, {
			{ t_pos4, "pos_trans" },
		}, {
		});
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
		switch (m_dim)
		{
		case 2:
			return "(pos_trans) = (proj) * (view) * (model) * vec4((pos), 0.0, 1.0);\n";
		case 3:
			return "(pos_trans) = (proj) * (view) * (model) * vec4((pos), 1.0);\n";
		case 4:
			return "(pos_trans) = (proj) * (view) * (model) * (pos);\n";
		default:
			return "";
		}
	}

private:
	int m_dim;

}; // PositionTrans

}
}
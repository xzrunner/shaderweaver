#pragma once

#include "shaderweaver/Node.h"

namespace sw
{
namespace node
{

class Rotate : public sw::Node
{
public:
	Rotate(bool radians)
		: Node("Rotate")
		, m_radians(radians)
	{
		AddVariable(Variable(t_uv | t_n_in, "uv"));
		AddVariable(Variable(t_flt2 | t_n_in, "center"));
		AddVariable(Variable(t_flt1 | t_n_in, "rotation"));

		AddVariable(Variable(t_uv | t_n_out, "rot"));

		AddVariable(Variable(t_uv | t_n_mid, "new_uv"));
		AddVariable(Variable(t_flt1 | t_n_mid, "s"));
		AddVariable(Variable(t_flt1 | t_n_mid, "c"));
		AddVariable(Variable(t_mat2 | t_n_mid, "rot_mat"));
	}

	enum ImportIdx
	{
		IN_UV = 0,
		IN_CENTER,
		IN_ROTATION,
	};

protected:
	virtual std::string GetBody() const override
	{
		return m_radians ?
			R"(
(new_uv) = (uv) - (center);
(s) = sin((rotation));
(c) = cos((rotation));
(rot_mat) = mat2((c), -(s), (s), (c));
(rot_mat) *= 0.5;
(rot_mat) += 0.5;
(rot_mat) = (rot_mat) * 2 - 1;
(new_uv) = (rot_mat) * (new_uv);
(new_uv) += (center);
(rot) = (new_uv);
)" + 1
		:
		R"(
(new_uv) = (uv) - (center);
(s) = sin((rotation) * (3.1415926f / 180.0f));
(c) = cos((rotation) * (3.1415926f / 180.0f));
(rot_mat) = mat2((c), -(s), (s), (c));
(rot_mat) *= 0.5;
(rot_mat) += 0.5;
(rot_mat) = (rot_mat) * 2 - 1;
(new_uv) = (rot_mat) * (new_uv);
(new_uv) += (center);
(rot) = (new_uv);
)" + 1;
	}

private:
	bool m_radians;

}; // Rotate

}
}
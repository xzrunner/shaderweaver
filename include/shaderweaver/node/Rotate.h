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
		InitVariables({
			{ t_uv, "uv" },
			{ t_flt2, "center" },
			{ t_flt1, "rotation" },
		}, {
			{ t_uv, "rot" },
		}, {
			{ t_uv, "new_uv" },
			{ t_flt1, "s" },
			{ t_flt1, "c" },
			{ t_mat2, "rot_mat" },
		});
	}

	enum InputID
	{
		ID_UV = 0,
		ID_CENTER,
		ID_ROTATION,
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
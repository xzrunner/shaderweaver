#pragma once

#include "shaderweaver/Node.h"

#include <SM_Vector.h>
#include <cpputil/StringHelper.h>

namespace sw
{
namespace node
{

class Flipbook : public sw::Node
{
public:
	Flipbook(const sm::bvec2& invert = { false, false })
		: Node("Flipbook")
		, m_invert(invert)
	{
		InitVariables({
			{ t_uv, "uv" },
			{ t_flt1, "width" },
			{ t_flt1, "height" },
			{ t_flt1, "tile" },
		}, {
			{ t_uv, "_out" },
		}, {
			{ t_flt1, "tile_fixed" },
			{ t_flt2, "tile_count" },
			{ t_flt1, "tile_y" },
			{ t_flt1, "tile_x" },
		});
	}

	void SetInvert(const sm::bvec2& invert) { m_invert = invert; }

	enum InputID
	{
		ID_UV = 0,
		ID_WIDTH,
		ID_HEIGHT,
		ID_TILE,
	};

protected:
	virtual std::string GetBody() const override
	{
		auto inv_x = m_invert.x ? 1 : 0;
		auto inv_y = m_invert.y ? 1 : 0;
		return cpputil::StringHelper::Format(
			R"(
#tile_fixed# = mod(#tile#, #width# * #height#);
#tile_count# = vec2(1.0, 1.0) / vec2(#width#, #height#);
#tile_y# = abs(%d * #height# - (floor(#tile_fixed# * #tile_count#.x) + %d * 1));
#tile_x# = abs(%d * #width# - ((#tile_fixed# - #width# * floor(#tile_fixed# * #tile_count#.x)) + %d * 1));
#_out# = (#uv# + vec2(#tile_x#, #tile_y#)) * #tile_count#;
)" + 1, inv_y, inv_y, inv_x, inv_x);
	}

private:
	sm::bvec2 m_invert;

	RTTR_ENABLE(Node)

}; // Flipbook

}
}
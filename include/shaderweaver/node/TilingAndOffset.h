#pragma once

#include "shaderweaver/Node.h"

namespace sw
{
namespace node
{

class TilingAndOffset : public sw::Node
{
public:
	TilingAndOffset()
		: Node("TilingAndOffset")
	{
		InitVariables({
			{ t_uv, "uv" },
			{ t_flt2, "tilling" },
			{ t_flt2, "offset" },
		}, {
			{ t_uv, "_out" },
		}, {
		});
	}

	enum InputID
	{
		ID_UV = 0,
		ID_TILLING,
		ID_OFFSET,
	};

protected:
	virtual std::string GetBody() const override
	{
		return "#_out# = #uv# * #tilling# + #offset#;\n";
	}

	RTTR_ENABLE(Node)

}; // TilingAndOffset

}
}
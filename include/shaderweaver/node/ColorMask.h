#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class ColorMask : public sw::Node
{
public:
	ColorMask()
		: Node("ColorMask")
	{
		InitVariables({
			{ t_col3, "_in" },
			{ t_col3, "mask" },
			{ t_flt1, "range" },
			{ t_flt1, "fuzziness" },
		}, {
			{ t_flt1, "_out" },
		}, {
			{ t_flt1, "dis" },
		});
	}

	enum InputID
	{
		ID_COL = 0,
		ID_MASK,
		ID_RANGE,
		ID_FUZZINESS,
	};

protected:
	virtual std::string GetBody() const override
	{
		return R"(
#dis# = distance(#mask#, #_in#);
#_out# = clamp(1 - (#dis# - #range#) / max(#fuzziness#, 1e-5), 0.0, 1.0);
)" + 1;
	}

	RTTR_ENABLE(Node)

}; // ColorMask

}
}
#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class ReplaceColor : public sw::Node
{
public:
	ReplaceColor()
		: Node("ReplaceColor")
	{
		InitVariables({
			{ t_col3, "_in" },
			{ t_col3, "from" },
			{ t_col3, "to" },
			{ t_flt1, "range" },
			{ t_flt1, "fuzziness" },
		}, {
			{ t_col3, "_out" },
		}, {
			{ t_flt1, "dis" },
		});
	}

	enum InputID
	{
		ID_COL = 0,
		ID_FROM,
		ID_TO,
		ID_RANGE,
		ID_FUZZINESS,
	};

protected:
	virtual std::string GetBody() const override
	{
		return R"(
#dis# = distance(#from#, #_in#);
#_out# = mix(#to#, #_in#, clamp((#dis# - #range#) / max(#fuzziness#, 1e-5f), 0.0, 1.0));
)" + 1;
	}

	RTTR_ENABLE(Node)

}; // ReplaceColor

}
}
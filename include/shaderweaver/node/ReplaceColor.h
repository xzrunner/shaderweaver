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
			{ t_col3, "col" },
			{ t_col3, "from" },
			{ t_col3, "to" },
			{ t_flt1, "range" },
			{ t_flt1, "fuzziness" },
		}, {
			{ t_col3, "replace_col" },
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
(dis) = distance((from), (col));
(replace_col) = mix((to), (col), clamp(((dis) - (range)) / max((fuzziness), 1e-5f), 0.0, 1.0));
)" + 1;
	}

}; // ReplaceColor

}
}
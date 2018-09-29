#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class Contrast : public sw::Node
{
public:
	Contrast()
		: Node("Contrast")
	{
		InitVariables({
			{ t_col3, "_in" },
			{ t_flt1, "contrast" },
		}, {
			{ t_col3, "_out" },
		}, {
			{ t_flt1, "midpoint" },
		});
	}

	enum InputID
	{
		ID_INPUT = 0,
		ID_CONTRAST,
	};

protected:
	virtual std::string GetBody() const override
	{
		return R"(
#midpoint# = pow(0.5, 2.2);
#_out# = (#_in# - #midpoint#) * #contrast# + #midpoint#;
)" + 1;
	}

}; // Contrast

}
}
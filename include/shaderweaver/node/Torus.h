#pragma once

#include "shaderweaver/Node.h"

namespace sw
{
namespace node
{

class Torus : public Node
{
public:
	Torus()
		: Node("Torus")
	{
		InitVariables({
			{ t_flt3, "pos" },
			{ t_flt2, "radius" },
		}, {
			{ t_flt1, "dis" },
		}, {
			{ t_flt2, "q" },
		});
	}

	enum InputID
	{
		ID_POS = 0,
		ID_RADIUS,
	};

protected:
	virtual std::string GetBody() const override
	{
		return R"(
#q# = vec2(length(#pos#.xz) - #radius#.x, #pos#.y);
#dis# = length(#q#) - #radius#.y;
)" + 1;
	}

}; // Torus

}
}
#pragma once

#include "shaderweaver/Node.h"

namespace sw
{
namespace node
{

class SDF : public Node
{
public:
	SDF()
		: Node("SDF")
	{
		InitVariables({
			{ t_flt3, "pos" },
		}, {
			{ t_flt1, "dis" },
		}, {
		});
	}

}; // SDF

}
}
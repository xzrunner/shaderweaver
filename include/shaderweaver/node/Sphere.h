#pragma once

#include "shaderweaver/Node.h"

namespace sw
{
namespace node
{

class Sphere : public Node
{
public:
	Sphere()
		: Node("Sphere")
	{
		InitVariables({
			{ t_flt3, "pos" },
			{ t_flt1, "radius" },
		}, {
			{ t_flt1, "dis" },
		}, {
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
#dis# = length(#pos#) - #radius#;
)" + 1;
	}

	RTTR_ENABLE(Node)

}; // Sphere

}
}
#pragma once

#include "shaderweaver/Node.h"

namespace sw
{
namespace node
{

class Box : public Node
{
public:
	Box()
		: Node("Box")
	{
		InitVariables({
			{ t_flt3, "pos" },
			{ t_flt3, "size" },
		}, {
			{ t_flt1, "dis" },
		}, {
            { t_flt3, "d" },
		});
	}

	enum InputID
	{
		ID_POS = 0,
		ID_SIZE,
	};

protected:
	virtual std::string GetBody() const override
	{
		return R"(
#d# = abs(#pos#) - #size#;
#dis# = length(max(#d#,0.0))
      + min(max(#d#.x,max(#d#.y,#d#.z)),0.0); // remove this line for an only partially signed sdf
)" + 1;
	}

	RTTR_ENABLE(Node)

}; // Box

}
}
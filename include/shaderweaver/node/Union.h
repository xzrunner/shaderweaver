#pragma once

#include "shaderweaver/Node.h"

namespace sw
{
namespace node
{

class Union : public Node
{
public:
	Union()
		: Node("Union")
	{
		InitVariables({
			{ t_flt1, "d1" },
			{ t_flt1, "d2" },
		}, {
			{ t_flt1, "_out" },
		}, {
		});
	}

	enum InputID
	{
		ID_D1 = 0,
        ID_D2,
	};

protected:
	virtual std::string GetBody() const override
	{
		return R"(
#_out# = min(#d1#, #d2#);
)" + 1;
	}

	RTTR_ENABLE(Node)

}; // Union

}
}
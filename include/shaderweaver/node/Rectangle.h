#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class Rectangle : public Node
{
public:
	Rectangle()
		: Node("Rectangle")
	{
		InitVariables({
			{ t_uv, "uv" },
			{ t_flt1, "width" },
			{ t_flt1, "height" },
		}, {
			{ t_flt1, "rectangle" },
		}, {
			{ t_flt2, "d" },
		});
	}


	enum InputID
	{
		ID_UV = 0,
		ID_WIDTH,
		ID_HEIGHT,
	};

protected:
	virtual std::string GetBody() const override
	{
		return R"(
(d) = abs((uv) * 2 - 1) - vec2((width), (height));
(d) = 1 - (d) / fwidth((d));
(rectangle) = clamp(min((d).x, (d).y), 0.0, 1.0);
)" + 1;
	}

}; // Rectangle

}
}
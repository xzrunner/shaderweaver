#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class UnpackScaleNormal : public sw::Node
{
public:
	UnpackScaleNormal()
		: Node("UnpackScaleNormal")
	{
		InitVariables({
			{ t_flt4, "value" },
			{ t_flt1, "scale" },
		}, {
			{ t_flt3, "xyz" },
			{ t_flt1, "x" },
            { t_flt1, "y" },
            { t_flt1, "z" },
		}, {
		});
	}

	enum InputID
	{
		ID_VALUE = 0,
		ID_SCALE
	};

	enum OutputID
	{
		ID_XYZ = 0,
		ID_X,
		ID_Y,
		ID_Z
	};

protected:
	virtual std::string GetBody() const override
	{
        // todo
        return "";
	}

	RTTR_ENABLE(Node)

}; // UnpackScaleNormal

}
}
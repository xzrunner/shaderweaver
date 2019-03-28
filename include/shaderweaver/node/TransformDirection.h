#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class TransformDirection : public sw::Node
{
public:
	TransformDirection()
		: Node("TransformDirection")
	{
		InitVariables({
			{ t_flt3, "_in" },
		}, {
            { t_flt3, "xyz" },
            { t_flt1, "x" },
            { t_flt1, "y" },
            { t_flt1, "z" },
		}, {
		});
	}

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

}; // TransformDirection

}
}
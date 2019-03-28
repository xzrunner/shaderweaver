#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class SampleTriplanar : public sw::Node
{
public:
	SampleTriplanar()
		: Node("SampleTriplanar")
	{
		InitVariables({
			{ t_tex2d, "tex" },
			{ t_flt2,  "tilling" },
            { t_flt1,  "falloff" }
		}, {
			{ t_col4 | t_read, "rgba" },
			{ t_flt1 | t_read, "r" },
			{ t_flt1 | t_read, "g" },
			{ t_flt1 | t_read, "b" },
			{ t_flt1 | t_read, "a" },
		}, {
		});
	}

	enum InputID
	{
		ID_TEX = 0,
		ID_TILLING,
        ID_FALLOFF
	};

	enum OutputID
	{
		ID_RGBA = 0,
		ID_R,
		ID_G,
		ID_B,
		ID_A,
	};

protected:
	virtual std::string GetBody() const override
	{
        // todo
        return "";
	}

	RTTR_ENABLE(Node)

}; // SampleTriplanar

}
}
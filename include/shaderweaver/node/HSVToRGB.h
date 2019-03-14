#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class HSVToRGB : public sw::Node
{
public:
    HSVToRGB()
        : Node("HSVToRGB")
    {
		InitVariables({
			{ t_flt1, "Hue" },
            { t_flt1, "Saturation" },
            { t_flt1, "Value" },
		}, {
			{ t_col3, "RGB" },
        }, {
            { t_flt4, "K" },
            { t_flt3, "P" },
        });
    }

protected:
    virtual std::string GetBody() const override
    {
        return R"(
#K# = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
//#P# = abs(fract(vec3(#Hue#, #Hue#, #Hue#) + #K#.xyz) * 6.0 - #K#.www);
#P# = abs(fract(#Hue#.xxx + #K#.xyz) * 6.0 - #K#.www);
#RGB# = #Value# * mix(#K#.xxx, clamp(#P# - #K#.xxx, 0.0, 1.0), #Saturation#);
)" + 1;
    }

    RTTR_ENABLE(Node)

}; // HSVToRGB

}
}
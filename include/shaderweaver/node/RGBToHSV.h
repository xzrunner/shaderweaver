#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class RGBToHSV : public sw::Node
{
public:
    RGBToHSV()
        : Node("RGBToHSV")
    {
		InitVariables({
            { t_col3, "RGB" },
		}, {
            { t_col3, "HSV" },
            { t_flt1, "Hue" },
            { t_flt1, "Saturation" },
            { t_flt1, "Value" },
        }, {
            { t_flt4, "K" },
            { t_flt4, "P" },
            { t_flt4, "Q" },
            { t_flt1, "D" },
            { t_flt1, "E" },
        });
    }

protected:
    virtual std::string GetBody() const override
    {
        return R"(
#K# = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
#P# = mix(vec4(#RGB#.bg, #K#.wz), vec4(#RGB#.gb, #K#.xy), step(#RGB#.b, #RGB#.g));
#Q# = mix(vec4(#P#.xyw, #RGB#.r), vec4(#RGB#.r, #P#.yzx), step(#P#.x, #RGB#.r));
#D# = #Q#.x - min(#Q#.w, #Q#.y);
#E# = 1e-10;
#Hue# = abs(#Q#.z + (#Q#.w - #Q#.y)/(6.0 * #D# + #E#));
#Saturation# = #D# / (#Q#.x + #E#);
#Value# = #Q#.x;
#HSV# = vec3(#Hue#, #Saturation#, #Value#);
)" + 1;
    }

    RTTR_ENABLE(Node)

}; // RGBToHSV

}
}
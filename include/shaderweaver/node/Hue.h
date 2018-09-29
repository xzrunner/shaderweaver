#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class Hue : public sw::Node
{
public:
	Hue(bool radians)
		: Node("Hue")
		, m_radians(radians)
	{
		InitVariables({
			{ t_col3, "_in" },
			{ t_flt1, "offset" },
		}, {
			{ t_col3, "_out" },
		}, {
			{ t_flt4, "k" },
			{ t_flt4, "p" },
			{ t_flt4, "q" },
			{ t_flt1, "d" },
			{ t_flt1, "e" },
			{ t_flt3, "hsv" },
			{ t_flt1, "hue" },
			{ t_flt4, "k2" },
			{ t_flt3, "p2" },
		});
	}

	enum InputID
	{
		ID_INPUT = 0,
		ID_OFFSET,
	};

protected:
	virtual std::string GetBody() const override
	{
		return m_radians ?
			R"(
#k# = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
#p# = mix(vec4(#_in#.bg, #k#.wz), vec4(#_in#.gb, #k#.xy), step(#_in#.b, #_in#.g));
#q# = mix(vec4(#p#.xyw, #_in#.r), vec4(#_in#.r, #p#.yzx), step(#p#.x, #_in#.r));
#d# = #q#.x - min(#q#.w, #q#.y);
#e# = 1e-10;
#hsv# = vec3(abs(#q#.z + (#q#.w - #q#.y)/(6.0 * #d# + #e#)), #d# / (#q#.x + #e#), #q#.x);

#hue# = #hsv#.x + #offset#;
#hsv#.x = (#hue# < 0)
        ? #hue# + 1
        : (#hue# > 1)
            ? #hue# - 1
            : #hue#;

// HSV to RGB
#k2# = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
#p2# = abs(fract(#hsv#.xxx + #k2#.xyz) * 6.0 - #k2#.www);
#_out# = #hsv#.z * mix(#k2#.xxx, clamp(#p2# - #k2#.xxx, 0.0, 1.0), #hsv#.y);
)" + 1
	:
			R"(
#k# = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
#p# = mix(vec4(#_in#.bg, #k#.wz), vec4(#_in#.gb, #k#.xy), step(#_in#.b, #_in#.g));
#q# = mix(vec4(#p#.xyw, #_in#.r), vec4(#_in#.r, #p#.yzx), step(#p#.x, #_in#.r));
#d# = #q#.x - min(#q#.w, #q#.y);
#e# = 1e-10;
#hsv# = vec3(abs(#q#.z + (#q#.w - #q#.y)/(6.0 * #d# + #e#)), #d# / (#q#.x + #e#), #q#.x);

#hue# = #hsv#.x + #offset# / 360;
#hsv#.x = (#hue# < 0)
        ? #hue# + 1
        : (#hue# > 1)
            ? #hue# - 1
            : #hue#;

#k2# = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
#p2# = abs(fract(#hsv#.xxx + #k2#.xyz) * 6.0 - #k2#.www);
#_out# = #hsv#.z * mix(#k2#.xxx, clamp(#p2# - #k2#.xxx, 0.0, 1.0), #hsv#.y);
)" + 1;
	}

private:
	bool m_radians;

}; // Hue

}
}
#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class EdgeDetect : public sw::Node
{
public:
	EdgeDetect()
		: Node("EdgeDetect")
	{
		InitVariables({
            { t_tex2d, "tex" },
            { t_uv,    "uv" },
            { t_flt1,  "blend" },
		}, {
			{ t_col4,  "_out" },
		}, {
		});
	}

    enum InputID
    {
        ID_TEX = 0,
        ID_UV,
        ID_BLEND,
    };

protected:
	virtual std::string GetBody() const override
	{
		return R"(
ivec2 ires = textureSize(#tex#, 0);
float ResS = float(ires.s);
float ResT = float(ires.t);
vec3 irgb = texture2D(#tex#, #uv#).rgb;

vec2 stp0 = vec2(1.0/ResS, 0.0);
vec2 st0p = vec2(0.0, 1.0/ResT);
vec2 stpp = vec2(1.0/ResS, 1.0/ResT);
vec2 stpm = vec2(1.0/ResS, -1.0/ResT);

const vec3 w = vec3(0.2125, 0.7154, 0.0721);
float i00 	= dot(texture2D(#tex#, #uv#).rgb, w);
float im1m1 = dot(texture2D(#tex#, #uv# - stpp).rgb, w);
float ip1p1 = dot(texture2D(#tex#, #uv# + stpp).rgb, w);
float im1p1 = dot(texture2D(#tex#, #uv# - stpm).rgb, w);
float ip1m1 = dot(texture2D(#tex#, #uv# + stpm).rgb, w);
float im10  = dot(texture2D(#tex#, #uv# - stp0).rgb, w);
float ip10  = dot(texture2D(#tex#, #uv# + stp0).rgb, w);
float i0m1  = dot(texture2D(#tex#, #uv# - st0p).rgb, w);
float i0p1  = dot(texture2D(#tex#, #uv# + st0p).rgb, w);

float h = -1.0 * im1p1 - 2.0 * i0p1 - 1.0 * ip1p1 + 1.0 * im1m1 + 2.0 * i0m1 + 1.0 * ip1m1;
float v = -1.0 * im1m1 - 2.0 * im10 - 1.0 * im1p1 + 1.0 * ip1m1 + 2.0 * ip10 + 1.0 * ip1p1;
float mag = length(vec2(h, v));
//vec3 target = vec3(mag, 0, 0);
vec3 target = vec3(mag, mag, mag);
//#_out# = vec4(mix(irgb, target, #blend#), texture2D(#tex#, #uv#).a);
//#_out# = vec4(mix(irgb, target, #blend#), 1.0);
#_out# = vec4(target * #blend#, 1.0);
)" + 1;
	}

	RTTR_ENABLE(Node)

}; // EdgeDetect

}
}
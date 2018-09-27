#pragma once

#include "shaderweaver/Node.h"

namespace sw
{
namespace node
{

class GradientNoise : public sw::Node
{
public:
	GradientNoise()
		: Node("GradientNoise")
	{
		InitVariables({
			{ t_uv, "uv" },
			{ t_flt1, "scale" },
		}, {
			{ t_flt1, "_out" },
		}, {
		});
	}

	enum InputID
	{
		ID_UV = 0,
		ID_SCALE,
	};

protected:
	virtual std::string GetHeader() const
	{
		return R"(
vec2 shaderweaver_gradient_noise_dir(vec2 p)
{
    p = mod(p, 289);
    float x = mod((34 * p.x + 1) * p.x, 289) + p.y;
    x = mod((34 * x + 1) * x, 289);
    x = fract(x / 41) * 2 - 1;
    return normalize(vec2(x - floor(x + 0.5), abs(x) - 0.5));
}

float shaderweaver_gradient_noise(vec2 p)
{
    vec2 ip = floor(p);
    vec2 fp = fract(p);
    float d00 = dot(shaderweaver_gradient_noise_dir(ip), fp);
    float d01 = dot(shaderweaver_gradient_noise_dir(ip + vec2(0, 1)), fp - vec2(0, 1));
    float d10 = dot(shaderweaver_gradient_noise_dir(ip + vec2(1, 0)), fp - vec2(1, 0));
    float d11 = dot(shaderweaver_gradient_noise_dir(ip + vec2(1, 1)), fp - vec2(1, 1));
    fp = fp * fp * fp * (fp * (fp * 6 - 15) + 10);
    return mix(mix(d00, d01, fp.y), mix(d10, d11, fp.y), fp.x);
}
)" + 1;
	}

	virtual std::string GetBody() const override
	{
		return R"(
#_out# = shaderweaver_gradient_noise(#uv# * #scale#) + 0.5;
)" + 1;
	}

}; // GradientNoise

}
}
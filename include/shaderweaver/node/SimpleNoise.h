#pragma once

#include "shaderweaver/Node.h"

namespace sw
{
namespace node
{

class SimpleNoise : public sw::Node
{
public:
	SimpleNoise()
		: Node("SimpleNoise")
	{
		InitVariables({
			{ t_uv, "uv" },
			{ t_flt1, "scale" },
		}, {
			{ t_flt1, "simple_noise" },
		}, {
			{ t_flt1, "t" },
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
float shaderweaver_noise_random_value(vec2 uv)
{
    return fract(sin(dot(uv, vec2(12.9898, 78.233)))*43758.5453);
}

float shaderweaver_noise_interpolate(float a, float b, float t)
{
    return (1.0 - t) * a + (t * b);
}

float shaderweaver_value_noise(vec2 uv)
{
    vec2 i = floor(uv);
    vec2 f = fract(uv);
    f = f * f * (3.0 - 2.0 * f);

    uv = abs(fract(uv) - 0.5);
    vec2 c0 = i + vec2(0.0, 0.0);
    vec2 c1 = i + vec2(1.0, 0.0);
    vec2 c2 = i + vec2(0.0, 1.0);
    vec2 c3 = i + vec2(1.0, 1.0);
    float r0 = shaderweaver_noise_random_value(c0);
    float r1 = shaderweaver_noise_random_value(c1);
    float r2 = shaderweaver_noise_random_value(c2);
    float r3 = shaderweaver_noise_random_value(c3);

    float bottom_of_grid = shaderweaver_noise_interpolate(r0, r1, f.x);
    float top_of_grid    = shaderweaver_noise_interpolate(r2, r3, f.x);
    float t = shaderweaver_noise_interpolate(bottom_of_grid, top_of_grid, f.y);
    return t;
}
)" + 1;
	}

	virtual std::string GetBody() const override
	{
		return R"(
(t) = 0.0;
for (int i = 0; i < 3; i++)
{
    float freq = pow(2.0, float(i));
    float amp = pow(0.5, float(3 - i));
    (t) += shaderweaver_value_noise(vec2((uv).x * (scale) / freq, (uv).y * (scale) / freq))*amp;
}
(simple_noise) = (t);
)" + 1;
	}

}; // SimpleNoise

}
}
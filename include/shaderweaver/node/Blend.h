#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"

namespace sw
{
namespace node
{

class Blend : public sw::Node
{
public:
	enum ModeType
	{
		MODE_BURN = 0,
		MODE_DARKEN,
		MODE_DIFFERENCE,
		MODE_DODGE,
		MODE_DIVIDE,
		MODE_EXCLUSION,
		MODE_HARD_LIGHT,
		MODE_HARD_MIX,
		MODE_LIGHTEN,
		MODE_LINEAR_BURN,
		MODE_LINEAR_DODGE,
		MODE_LINEAR_LIGHT,
		MODE_LINEAR_LIGHT_ADD_SUB,
		MODE_MULTIPLY,
		MODE_NEGATION,
		MODE_OVERLAY,
		MODE_PIN_LIGHT,
		MODE_SCREEN,
		MODE_SOFT_LIGHT,
		MODE_SUBTRACT,
		MODE_VIVID_LIGHT
	};

public:
	Blend(ModeType mode)
		: Node("Blend")
		, m_mode(mode)
	{
		InitVariables({
			{ t_flt3, "base" },
			{ t_flt3, "blend" },
			{ t_flt1, "opacity" },
		}, {
			{ t_flt3, "_out" },
		}, {
		});
	}

	enum InputID
	{
		ID_BASE = 0,
		ID_BLEND,
		ID_OPACITY,
	};

protected:
	virtual std::string GetHeader() const
	{
		return R"(
float shaderweaver_blend_hard_light(float base, float blend, float opacity)
{
	float result1 = 1.0 - 2.0 * (1.0 - base) * (1.0 - blend);
	float result2 = 2.0 * base * blend;
	float zeroOrOne = step(blend, 0.5);
	float ret = result2 * zeroOrOne + (1 - zeroOrOne) * result1;
	return mix(base, ret, opacity);
}

float shaderweaver_blend_hard_mix(float base, float blend, float opacity)
{
	float ret = step(1 - base, blend);
	return mix(base, ret, opacity);
}

float shaderweaver_blend_linear_light(float base, float blend, float opacity)
{
	float ret = blend < 0.5 ? max(base + (2 * blend) - 1, 0) : min(base + 2 * (blend - 0.5), 1);
	return mix(base, ret, opacity);
}

float shaderweaver_blend_overlay(float base, float blend, float opacity)
{
	float result1 = 1.0 - 2.0 * (1.0 - base) * (1.0 - blend);
	float result2 = 2.0 * base * blend;
	float zeroOrOne = step(base, 0.5);
	float ret = result2 * zeroOrOne + (1 - zeroOrOne) * result1;
	return mix(base, ret, opacity);
}

float shaderweaver_blend_pin_light(float base, float blend, float opacity)
{
	float check = step(0.5, blend);
	float result1 = check * max(2.0 * (base - 0.5), blend);
	float ret = result1 + (1.0 - check) * min(2.0 * base, blend);
	return mix(base, ret, opacity);
}

float shaderweaver_blend_soft_light(float base, float blend, float opacity)
{
	float result1 = 2.0 * base * blend + base * base * (1.0 - 2.0 * blend);
	float result2 = sqrt(base) * (2.0 * blend - 1.0) + 2.0 * base * (1.0 - blend);
	float zeroOrOne = step(0.5, blend);
	float ret = result2 * zeroOrOne + (1 - zeroOrOne) * result1;
	return mix(base, ret, opacity);
}

float shaderweaver_blend_vivid_light(float base, float blend, float opacity)
{
	float result1 = 1.0 - (1.0 - blend) / (2.0 * base);
	float result2 = blend / (2.0 * (1.0 - base));
	float zeroOrOne = step(0.5, base);
	float ret = result2 * zeroOrOne + (1 - zeroOrOne) * result1;
	return mix(base, ret, opacity);
}

)" + 1;
	}

	virtual std::string GetBody() const override
	{
		auto dim = GetImports()[0].var.Type().dim;
		switch (m_mode)
		{
		case MODE_BURN:
			return R"(
#_out# =  1.0 - (1.0 - #blend#) / #base#;
#_out# = mix(#base#, #_out#, #opacity#);
)" + 1;
		case MODE_DARKEN:
			return R"(
#_out# = min(#blend#, #base#);
#_out# = mix(#base#, #_out#, #opacity#);
)" + 1;
		case MODE_DIFFERENCE:
			return R"(
#_out# = abs(#blend# - #base#);
#_out# = mix(#base#, #_out#, #opacity#);
)" + 1;
		case MODE_DODGE:
			return R"(
#_out# = #base# / (1.0 - #blend#);
#_out# = mix(#base#, #_out#, #opacity#);
)" + 1;
		case MODE_DIVIDE:
			return R"(
#_out# = #base# / (#blend# + 0.000000000001);
#_out# = mix(#base#, #_out#, #opacity#);
)" + 1;
		case MODE_EXCLUSION:
			return R"(
#_out# = #blend# + #base# - (2.0 * #blend# * #base#);
#_out# = mix(#base#, #_out#, #opacity#);
)" + 1;
		case MODE_HARD_LIGHT:
			return R"(
#_out#.x = shaderweaver_blend_hard_light(#base#.x, #blend#.x, #opacity#);
#_out#.y = shaderweaver_blend_hard_light(#base#.y, #blend#.y, #opacity#);
#_out#.z = shaderweaver_blend_hard_light(#base#.z, #blend#.z, #opacity#);
)" + 1;
		case MODE_HARD_MIX:
			return R"(
#_out#.x = shaderweaver_blend_hard_mix(#base#.x, #blend#.x, #opacity#);
#_out#.y = shaderweaver_blend_hard_mix(#base#.y, #blend#.y, #opacity#);
#_out#.z = shaderweaver_blend_hard_mix(#base#.z, #blend#.z, #opacity#);
)" + 1;
		case MODE_LIGHTEN:
			return R"(
#_out# = max(#blend#, #base#);
#_out# = mix(#base#, #_out#, #opacity#);
)" + 1;
		case MODE_LINEAR_BURN:
			return R"(
#_out# = #base# + #blend# - 1.0;
#_out# = mix(#base#, #_out#, #opacity#);
)" + 1;
		case MODE_LINEAR_DODGE:
			return R"(
#_out# = #base# + #blend#;
#_out# = mix(#base#, #_out#, #opacity#);
)" + 1;
		case MODE_LINEAR_LIGHT:
			return R"(
#_out#.x = shaderweaver_blend_linear_light(#base#.x, #blend#.x, #opacity#);
#_out#.y = shaderweaver_blend_linear_light(#base#.y, #blend#.y, #opacity#);
#_out#.z = shaderweaver_blend_linear_light(#base#.z, #blend#.z, #opacity#);
)" + 1;
		case MODE_LINEAR_LIGHT_ADD_SUB:
			return R"(
#_out# = #blend# + 2.0 * #base# - 1.0;
#_out# = mix(#base#, #_out#, #opacity#);
)" + 1;
		case MODE_MULTIPLY:
			return R"(
#_out# = #base# * #blend#;
#_out# = mix(#base#, #_out#, #opacity#);
)" + 1;
		case MODE_NEGATION:
			return R"(
#_out# = 1.0 - abs(1.0 - #blend# - #base#);
#_out# = mix(#base#, #_out#, #opacity#);
)" + 1;
		case MODE_SCREEN:
			return R"(
#_out# = 1.0 - (1.0 - #blend#) * (1.0 - #base#);
#_out# = mix(#base#, #_out#, #opacity#);
)" + 1;
		case MODE_OVERLAY:
			return R"(
#_out#.x = shaderweaver_blend_overlay(#base#.x, #blend#.x, #opacity#);
#_out#.y = shaderweaver_blend_overlay(#base#.y, #blend#.y, #opacity#);
#_out#.z = shaderweaver_blend_overlay(#base#.z, #blend#.z, #opacity#);
)" + 1;
		case MODE_PIN_LIGHT:
			return R"(
#_out#.x = shaderweaver_blend_pin_light(#base#.x, #blend#.x, #opacity#);
#_out#.y = shaderweaver_blend_pin_light(#base#.y, #blend#.y, #opacity#);
#_out#.z = shaderweaver_blend_pin_light(#base#.z, #blend#.z, #opacity#);
)" + 1;
		case MODE_SOFT_LIGHT:
			return R"(
#_out#.x = shaderweaver_blend_soft_light(#base#.x, #blend#.x, #opacity#);
#_out#.y = shaderweaver_blend_soft_light(#base#.y, #blend#.y, #opacity#);
#_out#.z = shaderweaver_blend_soft_light(#base#.z, #blend#.z, #opacity#);
)" + 1;
		case MODE_VIVID_LIGHT:
			return R"(
#_out#.x = shaderweaver_blend_vivid_light(#base#.x, #blend#.x, #opacity#);
#_out#.y = shaderweaver_blend_vivid_light(#base#.y, #blend#.y, #opacity#);
#_out#.z = shaderweaver_blend_vivid_light(#base#.z, #blend#.z, #opacity#);
)" + 1;
		case MODE_SUBTRACT:
			return R"(
#_out# = #base# - #blend#;
#_out# = mix(#base#, #_out#, #opacity#);
)" + 1;
		}
	}

private:
	ModeType m_mode;

}; // Blend

}
}
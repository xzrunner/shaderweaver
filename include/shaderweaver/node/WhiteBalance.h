#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class WhiteBalance : public sw::Node
{
public:
	WhiteBalance()
		: Node("WhiteBalance")
	{
		InitVariables({
			{ t_flt3, "_in" },
			{ t_flt1, "temperature" },
			{ t_flt1, "tint" },
		}, {
			{ t_flt3, "_out" },
		}, {
		});
	}

	enum InputID
	{
		ID_INPUT = 0,
		ID_TEMPERATURE,
		ID_TINT,
	};

protected:
	virtual std::string GetHeader() const
	{
		return R"(
vec3 shaderweaver_white_balance(vec3 col, float temperature, float tint)
{
	// Range ~[-1.67;1.67] works best
	float t1 = temperature * 10 / 6;
	float t2 = tint * 10 / 6;

	// Get the CIE xy chromaticity of the reference white point.
	// Note: 0.31271 = x value on the D65 white point
	float x = 0.31271 - t1 * (t1 < 0 ? 0.1 : 0.05);
	float standardIlluminantY = 2.87 * x - 3 * x * x - 0.27509507;
	float y = standardIlluminantY + t2 * 0.05;

	// Calculate the coefficients in the LMS space.
	vec3 w1 = vec3(0.949237, 1.03542, 1.08728); // D65 white point

	// CIExyToLMS
	float Y = 1;
	float X = Y * x / y;
	float Z = Y * (1 - x - y) / y;
	float L = 0.7328 * X + 0.4296 * Y - 0.1624 * Z;
	float M = -0.7036 * X + 1.6975 * Y + 0.0061 * Z;
	float S = 0.0030 * X + 0.0136 * Y + 0.9834 * Z;
	vec3 w2 = vec3(L, M, S);

	vec3 balance = vec3(w1.x / w2.x, w1.y / w2.y, w1.z / w2.z);

	mat3 LIN_2_LMS_MAT = mat3(
		3.90405e-1, 5.49941e-1, 8.92632e-3,
		7.08416e-2, 9.63172e-1, 1.35775e-3,
		2.31082e-2, 1.28021e-1, 9.36245e-1
	);

	mat3 LMS_2_LIN_MAT = mat3(
		2.85847e+0, -1.62879e+0, -2.48910e-2,
		-2.10182e-1,  1.15820e+0,  3.24281e-4,
		-4.18120e-2, -1.18169e-1,  1.06867e+0
	);

	vec3 lms = LIN_2_LMS_MAT * col;
	lms *= balance;
	return LMS_2_LIN_MAT * lms;
}
)" + 1;
	}

	virtual std::string GetBody() const override
	{
		return "#_out# = shaderweaver_white_balance(#_in#, #temperature#, #tint#);\n";
	}

}; // WhiteBalance

}
}
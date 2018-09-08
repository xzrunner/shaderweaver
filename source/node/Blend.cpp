#include "sw/node/Blend.h"

namespace sw
{
namespace node
{

Blend::Blend()
	: Node("Blend")
{
	AddVariable(Variable(t_col4 | t_n_in, "base4"));
	AddVariable(Variable(t_col4 | t_n_in, "blend4"));
	AddVariable(Variable(t_col3 | t_n_mid, "base3"));
	AddVariable(Variable(t_col3 | t_n_mid, "blend3"));
	AddVariable(Variable(t_col3 | t_n_mid, "col3"));
	AddVariable(Variable(t_int1 | t_unif | t_n_in, "u_mode"));

	AddVariable(Variable(t_col4 | t_n_out, "col4"));
}

std::string Blend::GetHeader() const
{
	return R"(
/*
** Float blending modes
*/
#define BlendLinearDodgef(base, blend) 	BlendAddf(base, blend)
#define BlendLinearBurnf(base, blend) 	BlendSubstractf(base, blend)
#define BlendAddf(base, blend) 			min(base + blend, 1.0)
#define BlendSubstractf(base, blend) 	max(base + blend - 1.0, 0.0)
#define BlendLightenf(base, blend) 		max(blend, base)
#define BlendDarkenf(base, blend) 		min(blend, base)
#define BlendLinearLightf(base, blend) 	(blend < 0.5 ? BlendLinearBurnf(base, (2.0 * blend)) : BlendLinearDodgef(base, (2.0 * (blend - 0.5))))
#define BlendScreenf(base, blend) 		(1.0 - ((1.0 - base) * (1.0 - blend)))
#define BlendOverlayf(base, blend) 		(base < 0.5 ? (2.0 * base * blend) : (1.0 - 2.0 * (1.0 - base) * (1.0 - blend)))
#define BlendSoftLightf(base, blend) 	((blend < 0.5) ? (2.0 * base * blend + base * base * (1.0 - 2.0 * blend)) : (sqrt(base) * (2.0 * blend - 1.0) + 2.0 * base * (1.0 - blend)))
#define BlendColorDodgef(base, blend) 	((blend == 1.0) ? blend : min(base / (1.0 - blend), 1.0))
#define BlendColorBurnf(base, blend) 	((blend == 0.0) ? blend : max((1.0 - ((1.0 - base) / blend)), 0.0))
#define BlendVividLightf(base, blend) 	((blend < 0.5) ? BlendColorBurnf(base, (2.0 * blend)) : BlendColorDodgef(base, (2.0 * (blend - 0.5))))
#define BlendPinLightf(base, blend) 	((blend < 0.5) ? BlendDarkenf(base, (2.0 * blend)) : BlendLightenf(base, (2.0 *(blend - 0.5))))
#define BlendHardMixf(base, blend) 		((BlendVividLightf(base, blend) < 0.5) ? 0.0 : 1.0)
#define BlendReflectf(base, blend) 		((blend == 1.0) ? blend : min(base * base / (1.0 - blend), 1.0))

/*
** Vector3 blending modes
*/

// Component wise blending
#define Blend(base, blend, funcf) 		vec3(funcf(base.r, blend.r), funcf(base.g, blend.g), funcf(base.b, blend.b))

#define BlendNormal(base, blend) 		(blend)
#define BlendLighten(base, blend)		Blend(base, blend, BlendLightenf)
#define BlendDarken(base, blend)		Blend(base, blend, BlendDarkenf)
#define BlendMultiply(base, blend) 		(base * blend)
#define BlendAverage(base, blend) 		((base + blend) / 2.0)
#define BlendAdd(base, blend) 			min(base + blend, vec3(1.0, 1.0, 1.0))
#define BlendSubstract(base, blend) 	Blend(base, blend, BlendSubstractf)
#define BlendDifference(base, blend) 	abs(base - blend)
#define BlendNegation(base, blend) 		(vec3(1.0, 1.0, 1.0) - abs(vec3(1.0, 1.0, 1.0) - base - blend))
#define BlendExclusion(base, blend) 	(base + blend - 2.0 * base * blend)
#define BlendScreen(base, blend) 		Blend(base, blend, BlendScreenf)
#define BlendOverlay(base, blend) 		Blend(base, blend, BlendOverlayf)
#define BlendSoftLight(base, blend) 	Blend(base, blend, BlendSoftLightf)
#define BlendHardLight(base, blend) 	BlendOverlay(blend, base)
#define BlendColorDodge(base, blend) 	Blend(base, blend, BlendColorDodgef)
#define BlendColorBurn(base, blend) 	Blend(base, blend, BlendColorBurnf)
#define BlendLinearDodge(base, blend)	Blend(base, blend, BlendAddf)
#define BlendLinearBurn(base, blend)	Blend(base, blend, BlendLinearBurnf)
// Linear Light is another contrast-increasing mode
// If the blend color is darker than midgray, Linear Light darkens the image by decreasing the brightness. If the blend color is lighter than midgray, the result is a brighter image due to increased brightness.
#define BlendLinearLight(base, blend) 	Blend(base, blend, BlendLinearLightf)
#define BlendVividLight(base, blend) 	Blend(base, blend, BlendVividLightf)
#define BlendPinLight(base, blend) 		Blend(base, blend, BlendPinLightf)
#define BlendHardMix(base, blend) 		Blend(base, blend, BlendHardMixf)
#define BlendReflect(base, blend) 		Blend(base, blend, BlendReflectf)
#define BlendGlow(base, blend) 			BlendReflect(blend, base)
#define BlendPhoenix(base, blend) 		(min(base, blend) - max(base, blend) + vec3(1.0, 1.0, 1.0))
#define BlendOpacity(base, blend, F, O)	(F(base, blend) * O + blend * (1.0 - O))
)" + 1;
}

std::string Blend::GetBody() const
{
	return R"(
(base3)  = (base4).rgb / (base4).a;
(blend3) = (blend4).rgb / (blend4).a;

// normal
if (u_mode == 0) {
	(col3) = BlendNormal((base3), (blend3));
}
// darken modes
else if (u_mode == 10) {
	(col3) = BlendDarken((base3), (blend3));
} else if (u_mode == 11) {
	(col3) = BlendMultiply((base3), (blend3));
} else if (u_mode == 12) {
	(col3) = BlendColorBurn((base3), (blend3));
} else if (u_mode == 13) {
	(col3) = BlendLinearBurn((base3), (blend3));
}
// lighten modes
else if (u_mode == 20) {
	(col3) = BlendLighten((base3), (blend3));
} else if (u_mode == 21) {
	(col3) = BlendScreen((base3), (blend3));
} else if (u_mode == 22) {
	(col3) = BlendColorDodge((base3), (blend3));
} else if (u_mode == 23) {
	(col3) = BlendLinearDodge((base3), (blend3));
} else if (u_mode == 24) {
	(col3) = BlendLinearDodge((base3), (blend3));
}
// saturation modes
else if (u_mode == 30) {
	(col3) = BlendOverlay((base3), (blend3));
} else if (u_mode == 31) {
	(col3) = BlendSoftLight((base3), (blend3));
} else if (u_mode == 32) {
	(col3) = BlendHardLight((base3), (blend3));
} else if (u_mode == 33) {
	// todo: for "Too many vertex shader constants"
	(col3) = Blend((base3), (blend3), BlendVividLightf);
} else if (u_mode == 34) {
	// todo: for "Too many vertex shader constants"
	(col3) = Blend((base3), (blend3), BlendLinearLightf);
} else if (u_mode == 35) {
	(col3) = BlendPinLight((base3), (blend3));
} else if (u_mode == 36) {
	// fixed for shader link err on sumsung note3
//		(col3) = BlendHardMix((base3), (blend3));
}
// substraction modes
else if (u_mode == 40) {
	(col3) = BlendDifference((base3), (blend3));
} else if (u_mode == 41) {
	(col3) = BlendExclusion((base3), (blend3));
} else if (u_mode == 42) {
	(col3) = BlendExclusion((base3), (blend3));
}
// color modes

// others
else {
	(col3) = BlendNormal((base3), (blend3));
}

(col4) = vec4((col3) * (blend4).w, (blend4).w);
)" + 1;
}

}
}

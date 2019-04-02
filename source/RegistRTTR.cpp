
// artistic
#include "shaderweaver/node/Contrast.h"
#include "shaderweaver/node/Hue.h"
#include "shaderweaver/node/InvertColors.h"
#include "shaderweaver/node/ReplaceColor.h"
#include "shaderweaver/node/Saturation.h"
#include "shaderweaver/node/WhiteBalance.h"
#include "shaderweaver/node/Blend.h"
#include "shaderweaver/node/Gray.h"
#include "shaderweaver/node/ChannelMask.h"
#include "shaderweaver/node/ColorMask.h"
#include "shaderweaver/node/NormalBlend.h"
#include "shaderweaver/node/NormalCreate.h"
#include "shaderweaver/node/NormalStrength.h"
#include "shaderweaver/node/NormalUnpack.h"
#include "shaderweaver/node/ColorAddMul.h"
#include "shaderweaver/node/ColorMap.h"
#include "shaderweaver/node/ColorspaceConversion.h"
#include "shaderweaver/node/RGBToHSV.h"
#include "shaderweaver/node/HSVToRGB.h"
#include "shaderweaver/node/Tonemap.h"
#include "shaderweaver/node/GammaCorrect.h"
// channel
#include "shaderweaver/node/Combine.h"
#include "shaderweaver/node/Flip.h"
#include "shaderweaver/node/Split.h"
#include "shaderweaver/node/Swizzle.h"
// input
#include "shaderweaver/node/Boolean.h"
#include "shaderweaver/node/Time.h"
#include "shaderweaver/node/Vector1.h"
#include "shaderweaver/node/Vector2.h"
#include "shaderweaver/node/Vector3.h"
#include "shaderweaver/node/Vector4.h"
#include "shaderweaver/node/CameraPos.h"
#include "shaderweaver/node/TexCoords.h"
#include "shaderweaver/node/TextureTransform.h"
#include "shaderweaver/node/UV.h"
#include "shaderweaver/node/ViewDirection.h"
#include "shaderweaver/node/IndirectDiffuseLighting.h"
#include "shaderweaver/node/IndirectSpecularLight.h"
#include "shaderweaver/node/LightAttenuation.h"
#include "shaderweaver/node/LightColor.h"
#include "shaderweaver/node/WorldSpaceLightDir.h"
#include "shaderweaver/node/Matrix2.h"
#include "shaderweaver/node/Matrix3.h"
#include "shaderweaver/node/Matrix4.h"
#include "shaderweaver/node/SampleTex2D.h"
#include "shaderweaver/node/SampleTex3D.h"
#include "shaderweaver/node/SampleCube.h"
#include "shaderweaver/node/SampleTriplanar.h"
#include "shaderweaver/node/UnpackScaleNormal.h"
#include "shaderweaver/node/PI.h"
#include "shaderweaver/node/Color.h"
#include "shaderweaver/node/WorldBitangent.h"
#include "shaderweaver/node/WorldPosition.h"
#include "shaderweaver/node/WorldTangent.h"
#include "shaderweaver/node/VertexBitangent.h"
#include "shaderweaver/node/VertexNormal.h"
#include "shaderweaver/node/VertexTangent.h"
// master
#include "shaderweaver/node/PBR.h"
#include "shaderweaver/node/IBL.h"
#include "shaderweaver/node/FresnelSchlick.h"
#include "shaderweaver/node/FresnelSchlickRoughness.h"
#include "shaderweaver/node/Phong.h"
#include "shaderweaver/node/Raymarching.h"
// math
#include "shaderweaver/node/Absolute.h"
#include "shaderweaver/node/Exponential.h"
#include "shaderweaver/node/Length.h"
#include "shaderweaver/node/Log.h"
#include "shaderweaver/node/Modulo.h"
#include "shaderweaver/node/Negate.h"
#include "shaderweaver/node/Normalize.h"
#include "shaderweaver/node/Posterize.h"
#include "shaderweaver/node/Reciprocal.h"
#include "shaderweaver/node/ReciprocalSquareRoot.h"
#include "shaderweaver/node/Add.h"
#include "shaderweaver/node/Assign.h"
#include "shaderweaver/node/Divide.h"
#include "shaderweaver/node/Multiply.h"
#include "shaderweaver/node/Power.h"
#include "shaderweaver/node/SquareRoot.h"
#include "shaderweaver/node/Subtract.h"
#include "shaderweaver/node/DDX.h"
#include "shaderweaver/node/DDXY.h"
#include "shaderweaver/node/DDY.h"
#include "shaderweaver/node/FWidth.h"
#include "shaderweaver/node/InverseLerp.h"
#include "shaderweaver/node/Lerp.h"
#include "shaderweaver/node/Smoothstep.h"
#include "shaderweaver/node/MatrixConstruction.h"
#include "shaderweaver/node/MatrixDeterminant.h"
#include "shaderweaver/node/MatrixInverse.h"
#include "shaderweaver/node/MatrixOnlyRot.h"
#include "shaderweaver/node/MatrixSplit.h"
#include "shaderweaver/node/MatrixTranspose.h"
#include "shaderweaver/node/Clamp.h"
#include "shaderweaver/node/Fraction.h"
#include "shaderweaver/node/Maximum.h"
#include "shaderweaver/node/Minimum.h"
#include "shaderweaver/node/OneMinus.h"
#include "shaderweaver/node/RandomRange.h"
#include "shaderweaver/node/Remap.h"
#include "shaderweaver/node/Saturate.h"
#include "shaderweaver/node/Ceiling.h"
#include "shaderweaver/node/Floor.h"
#include "shaderweaver/node/Round.h"
#include "shaderweaver/node/Sign.h"
#include "shaderweaver/node/Step.h"
#include "shaderweaver/node/Truncate.h"
#include "shaderweaver/node/Arccosine.h"
#include "shaderweaver/node/Arcsine.h"
#include "shaderweaver/node/Arctangent.h"
#include "shaderweaver/node/Arctangent2.h"
#include "shaderweaver/node/Cosine.h"
#include "shaderweaver/node/DegreesToRadians.h"
#include "shaderweaver/node/HyperbolicCosine.h"
#include "shaderweaver/node/HyperbolicSine.h"
#include "shaderweaver/node/HyperbolicTangent.h"
#include "shaderweaver/node/RadiansToDegrees.h"
#include "shaderweaver/node/Sine.h"
#include "shaderweaver/node/Tangent.h"
#include "shaderweaver/node/CrossProduct.h"
#include "shaderweaver/node/Distance.h"
#include "shaderweaver/node/DotProduct.h"
#include "shaderweaver/node/Projection.h"
#include "shaderweaver/node/Rejection.h"
#include "shaderweaver/node/TransformDirection.h"
#include "shaderweaver/node/ScaleAndOffset.h"
// procedural
#include "shaderweaver/node/Checkerboard.h"
#include "shaderweaver/node/Ellipse.h"
#include "shaderweaver/node/GradientNoise.h"
#include "shaderweaver/node/Polygon.h"
#include "shaderweaver/node/Rectangle.h"
#include "shaderweaver/node/RoundedRectangle.h"
#include "shaderweaver/node/SimpleNoise.h"
#include "shaderweaver/node/Voronoi.h"
// sdf
#include "shaderweaver/node/Intersection.h"
#include "shaderweaver/node/Subtraction.h"
#include "shaderweaver/node/Union.h"
#include "shaderweaver/node/Sphere.h"
#include "shaderweaver/node/Torus.h"
#include "shaderweaver/node/Box.h"
#include "shaderweaver/node/EstimateNormal.h"
#include "shaderweaver/node/PhongIllumination.h"
#include "shaderweaver/node/SDF.h"
// utility
#include "shaderweaver/node/And.h"
#include "shaderweaver/node/Branch.h"
#include "shaderweaver/node/Comparison.h"
#include "shaderweaver/node/Not.h"
#include "shaderweaver/node/Or.h"
#include "shaderweaver/node/ViewMatrix.h"
#include "shaderweaver/node/FragmentShader.h"
#include "shaderweaver/node/VertexShader.h"
#include "shaderweaver/node/WorldNormalVector.h"
#include "shaderweaver/node/ShaderInput.h"
#include "shaderweaver/node/ShaderOutput.h"
#include "shaderweaver/node/ShaderUniform.h"
#include "shaderweaver/node/FuncInput.h"
#include "shaderweaver/node/FuncOutput.h"
#include "shaderweaver/node/FragPosTrans.h"
#include "shaderweaver/node/NormalTrans.h"
#include "shaderweaver/node/PositionTrans.h"
#include "shaderweaver/node/PositionTransOld.h"
#include "shaderweaver/node/VertexToFragment.h"
#include "shaderweaver/node/EncodeFloatRGBA.h"
#include "shaderweaver/node/DecodeFloatRGBA.h"
// uv
#include "shaderweaver/node/Flipbook.h"
#include "shaderweaver/node/PolarCoordinates.h"
#include "shaderweaver/node/RadialShear.h"
#include "shaderweaver/node/Rotate.h"
#include "shaderweaver/node/Spherize.h"
#include "shaderweaver/node/TilingAndOffset.h"
#include "shaderweaver/node/Twirl.h"
// tools
#include "shaderweaver/node/Custom.h"
#include "shaderweaver/node/Function.h"
#include "shaderweaver/node/Relay.h"
// anim
#include "shaderweaver/node/Skin.h"

#include <SM_Vector.h>

#define REGIST_NODE_TYPE(name)                                \
	rttr::registration::class_<sw::node::name>("sw::"#name)   \
		.constructor<>()                                      \
	;

#define REGIST_NODE_TYPE2(name, default_params)               \
	rttr::registration::class_<sw::node::name>("sw::"#name)   \
		.constructor<>()                                      \
		default_params                                        \
	;

RTTR_REGISTRATION
{

// artistic
REGIST_NODE_TYPE2(Contrast, (                                 \
	rttr::metadata((int)(sw::node::Contrast::ID_CONTRAST), 1) \
))
REGIST_NODE_TYPE(Hue)
REGIST_NODE_TYPE(InvertColors)
REGIST_NODE_TYPE(ReplaceColor)
REGIST_NODE_TYPE2(Saturation, (                                   \
	rttr::metadata((int)(sw::node::Saturation::ID_SATURATION), 1) \
))
REGIST_NODE_TYPE(WhiteBalance)
REGIST_NODE_TYPE2(Blend, (                                \
	rttr::metadata((int)(sw::node::Blend::ID_OPACITY), 1) \
))
REGIST_NODE_TYPE(Gray)
REGIST_NODE_TYPE(ChannelMask)
REGIST_NODE_TYPE(ColorMask)
REGIST_NODE_TYPE2(NormalBlend, (                                           \
	rttr::metadata((int)(sw::node::NormalBlend::ID_A), sm::vec3(0, 0, 1)), \
	rttr::metadata((int)(sw::node::NormalBlend::ID_B), sm::vec3(0, 0, 1))  \
))
REGIST_NODE_TYPE2(NormalCreate, (                                   \
	rttr::metadata((int)(sw::node::NormalCreate::ID_OFFSET), 0.5f), \
	rttr::metadata((int)(sw::node::NormalCreate::ID_STRENGTH), 8)   \
))
REGIST_NODE_TYPE2(NormalStrength, (                                               \
	rttr::metadata((int)(sw::node::NormalStrength::ID_INPUT), sm::vec3(0, 0, 1)), \
	rttr::metadata((int)(sw::node::NormalStrength::ID_STRENGTH), 1)               \
))
REGIST_NODE_TYPE(NormalUnpack)
REGIST_NODE_TYPE2(ColorAddMul, (                                                \
	rttr::metadata((int)(sw::node::ColorAddMul::ID_COL), sm::vec4(0, 0, 0, 1)), \
	rttr::metadata((int)(sw::node::ColorAddMul::ID_MUL), sm::vec4(1, 1, 1, 1)), \
	rttr::metadata((int)(sw::node::ColorAddMul::ID_ADD), sm::vec4(0, 0, 0, 0))  \
))
REGIST_NODE_TYPE2(ColorMap, (                                              \
	rttr::metadata((int)(sw::node::ColorMap::ID_COL),  sm::vec3(0, 0, 0)), \
	rttr::metadata((int)(sw::node::ColorMap::ID_RMAP), sm::vec3(1, 0, 0)), \
	rttr::metadata((int)(sw::node::ColorMap::ID_GMAP), sm::vec3(0, 1, 0)), \
	rttr::metadata((int)(sw::node::ColorMap::ID_BMAP), sm::vec3(0, 0, 1))  \
))
REGIST_NODE_TYPE(ColorspaceConversion)
REGIST_NODE_TYPE(RGBToHSV)
REGIST_NODE_TYPE(HSVToRGB)
REGIST_NODE_TYPE(GammaCorrect)
REGIST_NODE_TYPE(Tonemap)
// channel
REGIST_NODE_TYPE(Combine)
REGIST_NODE_TYPE(Flip)
REGIST_NODE_TYPE(Split)
REGIST_NODE_TYPE(Swizzle)
// input
REGIST_NODE_TYPE(Boolean)
REGIST_NODE_TYPE(Time)
REGIST_NODE_TYPE(Vector1)
REGIST_NODE_TYPE(Vector2)
REGIST_NODE_TYPE(Vector3)
REGIST_NODE_TYPE(Vector4)
REGIST_NODE_TYPE(TexCoords)
REGIST_NODE_TYPE(TextureTransform)
REGIST_NODE_TYPE(UV)
REGIST_NODE_TYPE(ViewDirection)
REGIST_NODE_TYPE(IndirectDiffuseLighting)
REGIST_NODE_TYPE(IndirectSpecularLight)
REGIST_NODE_TYPE(LightAttenuation)
REGIST_NODE_TYPE(LightColor)
REGIST_NODE_TYPE(WorldSpaceLightDir)
REGIST_NODE_TYPE(CameraPos)
REGIST_NODE_TYPE(Matrix2)
REGIST_NODE_TYPE(Matrix3)
REGIST_NODE_TYPE(Matrix4)
REGIST_NODE_TYPE(SampleTex2D)
REGIST_NODE_TYPE(SampleTex3D)
REGIST_NODE_TYPE(SampleCube)
REGIST_NODE_TYPE(SampleTriplanar)
REGIST_NODE_TYPE(UnpackScaleNormal)
REGIST_NODE_TYPE(PI)
REGIST_NODE_TYPE(Color)
REGIST_NODE_TYPE(WorldBitangent)
REGIST_NODE_TYPE(WorldPosition)
REGIST_NODE_TYPE(WorldTangent)
REGIST_NODE_TYPE(VertexBitangent)
REGIST_NODE_TYPE(VertexNormal)
REGIST_NODE_TYPE(VertexTangent)
// master
#ifdef SW_PBR_TEXTURE
REGIST_NODE_TYPE(PBR)
#else
REGIST_NODE_TYPE2(PBR, (                                                           \
	rttr::metadata((int)(sw::node::PBR::ID_ALBEDO), sm::vec3(0.5f, 0.0f, 0.0f)),   \
	rttr::metadata((int)(sw::node::PBR::ID_METALLIC), 0.5f),                       \
    rttr::metadata((int)(sw::node::PBR::ID_ROUGHNESS), 0.5f),                      \
    rttr::metadata((int)(sw::node::PBR::ID_AO), 1.0f)                              \
))
#endif // SW_PBR_TEXTURE
REGIST_NODE_TYPE(IBL)
REGIST_NODE_TYPE(FresnelSchlick)
REGIST_NODE_TYPE(FresnelSchlickRoughness)
REGIST_NODE_TYPE2(Phong, (                                                                \
	rttr::metadata((int)(sw::node::Phong::ID_LIT_POSITION),  sm::vec3(1.2f, 1.0f, 2.0f)), \
	rttr::metadata((int)(sw::node::Phong::ID_LIT_AMBIENT),   sm::vec3(0.2f, 0.2f, 0.2f)), \
	rttr::metadata((int)(sw::node::Phong::ID_LIT_DIFFUSE),   sm::vec3(0.5f, 0.5f, 0.5f)), \
	rttr::metadata((int)(sw::node::Phong::ID_LIT_SPECULAR),  sm::vec3(1.0f, 1.0f, 1.0f)), \
	rttr::metadata((int)(sw::node::Phong::ID_MAT_DIFFUSE),   sm::vec3(1, 0, 0)),          \
	rttr::metadata((int)(sw::node::Phong::ID_MAT_SPECULAR),  sm::vec3(0, 0.5f, 0)),       \
	rttr::metadata((int)(sw::node::Phong::ID_MAT_SHININESS), 64.0f),                      \
	rttr::metadata((int)(sw::node::Phong::ID_MAT_EMISSION),  sm::vec3(0, 0, 0))           \
))
REGIST_NODE_TYPE(Raymarching)
// math
REGIST_NODE_TYPE(Absolute)
REGIST_NODE_TYPE(Exponential)
REGIST_NODE_TYPE(Length)
REGIST_NODE_TYPE(Log)
REGIST_NODE_TYPE(Modulo)
REGIST_NODE_TYPE(Negate)
REGIST_NODE_TYPE(Normalize)
REGIST_NODE_TYPE(Posterize)
REGIST_NODE_TYPE(Reciprocal)
REGIST_NODE_TYPE(ReciprocalSquareRoot)
REGIST_NODE_TYPE(Add)
REGIST_NODE_TYPE(Assign)
REGIST_NODE_TYPE(Divide)
REGIST_NODE_TYPE(Multiply)
REGIST_NODE_TYPE(Power)
REGIST_NODE_TYPE(SquareRoot)
REGIST_NODE_TYPE(Subtract)
REGIST_NODE_TYPE(DDX)
REGIST_NODE_TYPE(DDXY)
REGIST_NODE_TYPE(DDY)
REGIST_NODE_TYPE(FWidth)
REGIST_NODE_TYPE(InverseLerp)
REGIST_NODE_TYPE(Lerp)
REGIST_NODE_TYPE(Smoothstep)
REGIST_NODE_TYPE(MatrixConstruction)
REGIST_NODE_TYPE(MatrixDeterminant)
REGIST_NODE_TYPE(MatrixInverse)
REGIST_NODE_TYPE(MatrixOnlyRot)
REGIST_NODE_TYPE(MatrixSplit)
REGIST_NODE_TYPE(MatrixTranspose)
REGIST_NODE_TYPE(Clamp)
REGIST_NODE_TYPE(Fraction)
REGIST_NODE_TYPE(Maximum)
REGIST_NODE_TYPE(Minimum)
REGIST_NODE_TYPE2(OneMinus, ( \
	rttr::metadata(0, 1)      \
))
REGIST_NODE_TYPE2(RandomRange, ( \
	rttr::metadata(0, 1)         \
))
REGIST_NODE_TYPE2(Remap, (                                            \
	rttr::metadata((int)(sw::node::Remap::ID_FROM), sm::vec2(-1, 1)), \
	rttr::metadata((int)(sw::node::Remap::ID_TO),   sm::vec2( 0, 1))  \
))
REGIST_NODE_TYPE(Saturate)
REGIST_NODE_TYPE(Ceiling)
REGIST_NODE_TYPE(Floor)
REGIST_NODE_TYPE(Round)
REGIST_NODE_TYPE(Sign)
REGIST_NODE_TYPE2(Step, (                             \
	rttr::metadata((int)(sw::node::Step::ID_EDGE), 1) \
))
REGIST_NODE_TYPE(Truncate)
REGIST_NODE_TYPE(Arccosine)
REGIST_NODE_TYPE(Arcsine)
REGIST_NODE_TYPE(Arctangent)
REGIST_NODE_TYPE(Arctangent2)
REGIST_NODE_TYPE(Cosine)
REGIST_NODE_TYPE(DegreesToRadians)
REGIST_NODE_TYPE(HyperbolicCosine)
REGIST_NODE_TYPE(HyperbolicSine)
REGIST_NODE_TYPE(HyperbolicTangent)
REGIST_NODE_TYPE(RadiansToDegrees)
REGIST_NODE_TYPE(Sine)
REGIST_NODE_TYPE(Tangent)
REGIST_NODE_TYPE(CrossProduct)
REGIST_NODE_TYPE(Distance)
REGIST_NODE_TYPE(DotProduct)
REGIST_NODE_TYPE(Projection)
REGIST_NODE_TYPE(Rejection)
REGIST_NODE_TYPE(TransformDirection)
REGIST_NODE_TYPE(ScaleAndOffset)
// procedural
REGIST_NODE_TYPE2(Checkerboard, (                                                            \
	rttr::metadata((int)(sw::node::Checkerboard::ID_COLOR_A),   sm::vec3(0.2f, 0.2f, 0.2f)), \
	rttr::metadata((int)(sw::node::Checkerboard::ID_COLOR_B),   sm::vec3(0.7f, 0.7f, 0.7f)), \
	rttr::metadata((int)(sw::node::Checkerboard::ID_FREQUENCY), sm::vec2(1, 1))              \
))
REGIST_NODE_TYPE2(Ellipse, (                                   \
	rttr::metadata((int)(sw::node::Ellipse::ID_WIDTH),  0.5f), \
	rttr::metadata((int)(sw::node::Ellipse::ID_HEIGHT), 0.5f)  \
))
REGIST_NODE_TYPE2(GradientNoise, (                               \
	rttr::metadata((int)(sw::node::GradientNoise::ID_SCALE), 10) \
))
REGIST_NODE_TYPE2(Polygon, (                                   \
	rttr::metadata((int)(sw::node::Polygon::ID_SIDES),  6),    \
	rttr::metadata((int)(sw::node::Polygon::ID_WIDTH),  0.5f), \
	rttr::metadata((int)(sw::node::Polygon::ID_HEIGHT), 0.5f)  \
))
REGIST_NODE_TYPE2(Rectangle, (                                   \
	rttr::metadata((int)(sw::node::Rectangle::ID_WIDTH),  0.5f), \
	rttr::metadata((int)(sw::node::Rectangle::ID_HEIGHT), 0.5f)  \
))
REGIST_NODE_TYPE2(RoundedRectangle, (                                   \
	rttr::metadata((int)(sw::node::RoundedRectangle::ID_WIDTH),  0.5f), \
	rttr::metadata((int)(sw::node::RoundedRectangle::ID_HEIGHT), 0.5f), \
	rttr::metadata((int)(sw::node::RoundedRectangle::ID_RADIUS), 0.1f)  \
))
REGIST_NODE_TYPE2(SimpleNoise, (                                \
	rttr::metadata((int)(sw::node::SimpleNoise::ID_SCALE), 500) \
))
REGIST_NODE_TYPE2(Voronoi, (                                      \
	rttr::metadata((int)(sw::node::Voronoi::ID_ANGLE_OFFSET), 2), \
	rttr::metadata((int)(sw::node::Voronoi::ID_CELL_DENSITY), 5)  \
))
// sdf
REGIST_NODE_TYPE(Intersection)
REGIST_NODE_TYPE(Subtraction)
REGIST_NODE_TYPE(Union)
REGIST_NODE_TYPE2(Sphere, (                                  \
	rttr::metadata((int)(sw::node::Sphere::ID_RADIUS), 0.5f) \
))
REGIST_NODE_TYPE2(Torus, (                                                  \
	rttr::metadata((int)(sw::node::Torus::ID_RADIUS), sm::vec2(1.6f, 0.6f)) \
))
REGIST_NODE_TYPE2(Box, (                                                      \
	rttr::metadata((int)(sw::node::Box::ID_SIZE), sm::vec3(1.0f, 1.0f, 1.0f)) \
))
REGIST_NODE_TYPE(EstimateNormal)
REGIST_NODE_TYPE(PhongIllumination)
REGIST_NODE_TYPE(SDF)
// utility
REGIST_NODE_TYPE(And)
REGIST_NODE_TYPE(Branch)
REGIST_NODE_TYPE(Comparison)
REGIST_NODE_TYPE(Not)
REGIST_NODE_TYPE(Or)
REGIST_NODE_TYPE(ViewMatrix)
REGIST_NODE_TYPE(FragmentShader)
REGIST_NODE_TYPE(VertexShader)
REGIST_NODE_TYPE(WorldNormalVector)
REGIST_NODE_TYPE(ShaderInput)
REGIST_NODE_TYPE(ShaderOutput)
REGIST_NODE_TYPE(ShaderUniform)
REGIST_NODE_TYPE(FuncInput)
REGIST_NODE_TYPE(FuncOutput)
REGIST_NODE_TYPE(FragPosTrans)
REGIST_NODE_TYPE(NormalTrans)
REGIST_NODE_TYPE(PositionTrans)
REGIST_NODE_TYPE(PositionTransOld)
REGIST_NODE_TYPE(VertexToFragment)
REGIST_NODE_TYPE(EncodeFloatRGBA)
REGIST_NODE_TYPE(DecodeFloatRGBA)
// uv
REGIST_NODE_TYPE2(Flipbook, (                                \
	rttr::metadata((int)(sw::node::Flipbook::ID_WIDTH),  1), \
	rttr::metadata((int)(sw::node::Flipbook::ID_HEIGHT), 1), \
	rttr::metadata((int)(sw::node::Flipbook::ID_TILE),   0)  \
))
REGIST_NODE_TYPE2(PolarCoordinates, (                                                         \
	rttr::metadata((int)(sw::node::PolarCoordinates::ID_CENTER),       sm::vec2(0.5f, 0.5f)), \
	rttr::metadata((int)(sw::node::PolarCoordinates::ID_RADIAL_SCALE), 1),                    \
	rttr::metadata((int)(sw::node::PolarCoordinates::ID_LENGTH_SCALE), 1)                     \
))
REGIST_NODE_TYPE2(RadialShear, (                                                     \
	rttr::metadata((int)(sw::node::RadialShear::ID_CENTER),   sm::vec2(0.5f, 0.5f)), \
	rttr::metadata((int)(sw::node::RadialShear::ID_STRENGTH), sm::vec2(10, 10))      \
))
REGIST_NODE_TYPE2(Rotate, (                                                  \
	rttr::metadata((int)(sw::node::Rotate::ID_CENTER), sm::vec2(0.5f, 0.5f)) \
))
REGIST_NODE_TYPE2(Spherize, (                                                     \
	rttr::metadata((int)(sw::node::Spherize::ID_CENTER),   sm::vec2(0.5f, 0.5f)), \
	rttr::metadata((int)(sw::node::Spherize::ID_STRENGTH), sm::vec2(10, 10))      \
))
REGIST_NODE_TYPE2(TilingAndOffset, (                                             \
	rttr::metadata((int)(sw::node::TilingAndOffset::ID_TILLING), sm::vec2(1, 1)) \
))
REGIST_NODE_TYPE2(Twirl, (                                                     \
	rttr::metadata((int)(sw::node::Twirl::ID_CENTER),   sm::vec2(0.5f, 0.5f)), \
	rttr::metadata((int)(sw::node::Twirl::ID_STRENGTH), 10)                    \
))
// tools
REGIST_NODE_TYPE(Custom)
REGIST_NODE_TYPE(Function)
REGIST_NODE_TYPE(Relay)
// anim
REGIST_NODE_TYPE(Skin)

}

namespace sw
{

void regist_rttr()
{
}

}

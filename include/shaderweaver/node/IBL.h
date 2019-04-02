#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class IBL : public sw::Node
{
public:
	IBL()
		: Node("IBL")
	{
		InitVariables({
			{ t_flt3, "N" },
            { t_flt3, "V" },
            { t_flt3, "F0" },
            { t_flt3, "albedo" },
            { t_flt1, "metallic" },
            { t_flt1, "roughness" },
            { t_flt1, "ao" },
            { t_tex_cube, "irradiance_map" },
            { t_tex_cube, "prefilter_map" },
            { t_tex2d,    "brdf_lut" },
		}, {
			{ t_col3, "_out" },
		}, {
            { t_flt3, "F" },
            { t_flt3, "kS" },
            { t_flt3, "kD" },
            { t_flt3, "irradiance" },
            { t_flt3, "diffuse" },
		});
	}

    static const char* IrradianceMapName() { return "irradiance_map"; }

    enum InputID
    {
        ID_N,
        ID_V,
        ID_F0,
        ID_ALBEDO,
        ID_METALLIC,
        ID_ROUGHNESS,
        ID_AO,
        ID_IRRADIANCE_MAP,
        ID_PREFILTER_MAP,
        ID_BRDF_LUT,
    };

protected:
    virtual std::string GetHeader() const override
    {
        return R"(
vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}
)" + 1;
    }

	virtual std::string GetBody() const override
	{
		return R"(
#F# = FresnelSchlickRoughness(max(dot(#N#, #V#), 0.0), #F0#, #roughness#);

#kS# = #F#;
#kD# = 1.0 - #kS#;
#kD# *= 1.0 - #metallic#;

#irradiance# = textureCube(#irradiance_map#, #N#).rgb;
#diffuse# = #irradiance# * #albedo#;

//// sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
//const float MAX_REFLECTION_LOD = 4.0;
//vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;
//vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
//vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

//// ambient
//#_out# = (kD * diffuse + specular) * ao;

#_out# = (kD * diffuse) * ao;
)" + 1;
	}

	RTTR_ENABLE(Node)

}; // IBL

}
}
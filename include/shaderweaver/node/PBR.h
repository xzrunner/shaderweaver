#pragma once

#include "shaderweaver/Node.h"

namespace sw
{
namespace node
{

class PBR : public sw::Node
{
public:
    PBR()
        : Node("PBR")
    {
		InitVariables({
			{ t_flt2, "tex_coords" },
            { t_flt3, "world_pos", false },
            { t_flt3, "normal", false },
            { t_flt3, "albedo" },
            { t_flt1, "metallic" },
            { t_flt1, "roughness" },
            { t_flt1, "ao" },
            { t_flt3, "light_positions" },
            { t_flt3, "light_colors" },
            { t_flt3, "cam_pos" },
		}, {
			{ t_col3, "_out" },
		}, {
		});

        SetArray(ID_LIGHT_POSITIONS, 4);
        SetArray(ID_LIGHT_COLORS, 4);
    }

    enum InputID
    {
        ID_TEX_COORDS = 0,
        ID_FRAG_POS,
        ID_NORMAL,
        ID_ALBEDO,
        ID_METALLIC,
        ID_ROUGHNESS,
        ID_AO,
        ID_LIGHT_POSITIONS,
        ID_LIGHT_COLORS,
        ID_CAM_POS,
    };

protected:
    virtual std::string GetHeader() const override
    {
        return R"(

const float PI = 3.14159265359;

uniform vec3 light_positions[4] = {
        vec3(-10.0f,  10.0f, 10.0f),
        vec3( 10.0f,  10.0f, 10.0f),
        vec3(-10.0f, -10.0f, 10.0f),
        vec3( 10.0f, -10.0f, 10.0f),
};
uniform vec3 light_colors[4] = {
        vec3(300.0f, 300.0f, 300.0f),
        vec3(300.0f, 300.0f, 300.0f),
        vec3(300.0f, 300.0f, 300.0f),
        vec3(300.0f, 300.0f, 300.0f)
};

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / max(denom, 0.001); // prevent divide by zero for roughness=0.0 and NdotH=1.0
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

)" + 1;
    }

    virtual std::string GetBody() const override
    {
        return R"(
vec3 N = normalize(#normal#);
vec3 V = normalize(#cam_pos# - #world_pos#);

// calculate reflectance at normal incidence; if dia-electric (like plastic) use F0
// of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)
vec3 F0 = vec3(0.04);
F0 = mix(F0, #albedo#, #metallic#);

// reflectance equation
vec3 Lo = vec3(0.0);
for(int i = 0; i < 4; ++i)
{
    // calculate per-light radiance
    vec3 L = normalize(light_positions[i] - #world_pos#);
    vec3 H = normalize(V + L);
    float distance = length(light_positions[i] - #world_pos#);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = light_colors[i] * attenuation;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, #roughness#);
    float G   = GeometrySmith(N, V, L, #roughness#);
    vec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

    vec3 nominator    = NDF * G * F;
    float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
    vec3 specular = nominator / max(denominator, 0.001); // prevent divide by zero for NdotV=0.0 or NdotL=0.0

    // kS is equal to Fresnel
    vec3 kS = F;
    // for energy conservation, the diffuse and specular light can't
    // be above 1.0 (unless the surface emits light); to preserve this
    // relationship the diffuse component (kD) should equal 1.0 - kS.
    vec3 kD = vec3(1.0) - kS;
    // multiply kD by the inverse metalness such that only non-metals
    // have diffuse lighting, or a linear blend if partly metal (pure metals
    // have no diffuse light).
    kD *= 1.0 - #metallic#;

    // scale light by NdotL
    float NdotL = max(dot(N, L), 0.0);

    // add to outgoing radiance Lo
    Lo += (kD * #albedo# / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
}

// ambient lighting (note that the next IBL tutorial will replace
// this ambient lighting with environment lighting).
vec3 ambient = vec3(0.03) * #albedo# * #ao#;

vec3 color = ambient + Lo;

// HDR tonemapping
color = color / (color + vec3(1.0));
// gamma correct
color = pow(color, vec3(1.0/2.2));

#_out# = color;
)" + 1;
    }

private:
    void SetArray(int idx, int dim)
    {
        auto type = m_imports[idx].var.GetType();
        type.SetArray(dim);
        m_imports[idx].var.SetType(type);
    }

    RTTR_ENABLE(Node)

}; // PBR

}
}
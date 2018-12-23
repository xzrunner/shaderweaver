// code from https://www.shadertoy.com/view/lt33z7

#pragma once

#include "shaderweaver/node/SDF.h"
#include "shaderweaver/node/EstimateNormal.h"

namespace sw
{
namespace node
{

class PhongIllumination : public Node
{
public:
	PhongIllumination()
		: Node("PhongIllumination")
	{
		InitVariables({
			{ t_flt3, "k_a" },		// Ambient color
			{ t_flt3, "k_d" },		// Diffuse color
			{ t_flt3, "k_s" },		// Specular color
			{ t_flt1, "alpha" },	// Shininess coefficient
			{ t_flt3, "p" },		// position of point being lit
			{ t_flt3, "eye" },		// the position of the camera

			{ t_func, "sdf" },
			{ t_func, "normal" },
		}, {
			{ t_flt3, "color" },
		}, {
		});

		AddNesting("sdf", std::make_shared<SDF>());
		AddNesting("normal", std::make_shared<EstimateNormal>());
	}

protected:
	virtual std::string GetHeader() const override
	{
		return R"(
vec3 phong_contrib_for_light(vec3 k_d, vec3 k_s, float alpha, vec3 p, vec3 eye,
                             vec3 light_pos, vec3 light_intensity)
{
    vec3 N = #normal#(p, #sdf#);
    vec3 L = normalize(light_pos - p);
    vec3 V = normalize(eye - p);
    vec3 R = normalize(reflect(-L, N));

    float dotLN = dot(L, N);
    float dotRV = dot(R, V);

    if (dotLN < 0.0) {
        // Light not visible from this point on the surface
        return vec3(0.0, 0.0, 0.0);
    }

    if (dotRV < 0.0) {
        // Light reflection in opposite direction as viewer, apply only diffuse
        // component
        return light_intensity * (k_d * dotLN);
    }
    return light_intensity * (k_d * dotLN + k_s * pow(dotRV, alpha));
}

vec3 phong_illumination(vec3 k_a, vec3 k_d, vec3 k_s, float alpha, vec3 p, vec3 eye)
{
    const vec3 ambient_light = 0.5 * vec3(1.0, 1.0, 1.0);
    vec3 color = ambient_light * k_a;

    vec3 light1_pos = vec3(4.0 * sin(iTime),
                           2.0,
                           4.0 * cos(iTime));
    vec3 light1_intensity = vec3(0.4, 0.4, 0.4);

    color += phong_contrib_for_light(k_d, k_s, alpha, p, eye, light1_pos, light1_intensity);

    vec3 light2_pos = vec3(2.0 * sin(0.37 * iTime),
                           2.0 * cos(0.37 * iTime),
                           2.0);
    vec3 light2_intensity = vec3(0.4, 0.4, 0.4);

    color += phong_contrib_for_light(k_d, k_s, alpha, p, eye, light2_pos, light2_intensity);

    return color;
}

)" + 1;
	}

	virtual std::string GetBody() const override
	{
		return R"(
#color# = phong_illumination(#k_a#, #k_d#, #k_s#, #alpha#, #p#, #eye#);
)" + 1;
	}

	RTTR_ENABLE(Node)

}; // PhongIllumination

}
}
// code from https://www.shadertoy.com/view/Xtd3z7

#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"
// todo
#include "shaderweaver/node/PhongIllumination.h"

namespace sw
{
namespace node
{

class Raymarching : public sw::Node
{
public:
	Raymarching()
		: Node("Raymarching")
	{
		InitVariables({
            { t_uv,   "uv" }, // todo NodePreview::Update
			{ t_func, "sdf" },
			{ t_func, "lighting" },
		}, {
			{ t_flt4, "_out" },
		}, {
            { t_flt2 | t_unif, ResolutionName() },
            { t_flt3, "view_dir" },
			{ t_flt3, "eye" },
			{ t_flt1, "dist" },
		});

		AddNesting("sdf", "sw::SDF");
		AddNesting("lighting", "sw::PhongIllumination");

        m_nest_cb = [&]()
        {
            assert(m_imports[ID_SDF].conns.size() == 1);
            auto from = m_imports[ID_SDF].conns[0].node.lock();
            assert(from);

            assert(m_imports[ID_LIGHTING].conns.size() == 1);
            auto to = m_imports[ID_LIGHTING].conns[0].node.lock();
            assert(to);

            make_connecting({ from, 0 }, { to, PhongIllumination::ID_SDF });
        };
	}

	enum InputID
	{
        ID_UV = 0, // todo
		ID_SDF,
		ID_LIGHTING,
	};

    static const char* ResolutionName() { return "u_resolution"; }

protected:
	virtual std::string GetHeader() const override
	{
		return R"(

const int MAX_MARCHING_STEPS = 255;
const float MIN_DIST = 0.0;
const float MAX_DIST = 100.0;
const float EPSILON_RM = 0.0001;

/**
 * Return the shortest distance from the eyepoint to the scene surface along
 * the marching direction. If no part of the surface is found between start and end,
 * return end.
 *
 * eye: the eye point, acting as the origin of the ray
 * marchingDirection: the normalized direction to march in
 * start: the starting distance away from the eye
 * end: the max distance away from the ey to march before giving up
 */
float shortest_distance_to_surface(vec3 eye, vec3 marchingDirection, float start, float end)
{
    float depth = start;
    for (int i = 0; i < MAX_MARCHING_STEPS; i++) {
        float dist = #sdf#(eye + depth * marchingDirection);
        if (dist < EPSILON_RM) {
            return depth;
        }
        depth += dist;
        if (depth >= end) {
            return end;
        }
    }
    return end;
}

/**
 * Return the normalized direction to march in from the eye point for a single pixel.
 *
 * fieldOfView: vertical field of view in degrees
 * size: resolution of the output image
 * fragCoord: the x,y coordinate of the pixel in the output image
 */
vec3 ray_direction(float field_of_view, vec2 size, vec2 frag_coord)
{
    vec2 xy = frag_coord - size / 2.0;
    float z = size.y / tan(radians(field_of_view) / 2.0);
    return normalize(vec3(xy, -z));
}

/**
 * Return a transform matrix that will transform a ray from view space
 * to world coordinates, given the eye point, the camera target, and an up vector.
 *
 * This assumes that the center of the camera is aligned with the negative z axis in
 * view space when calculating the ray marching direction. See rayDirection.
 */
mat4 view_matrix(vec3 eye, vec3 center, vec3 up) {
    // Based on gluLookAt man page
    vec3 f = normalize(center - eye);
    vec3 s = normalize(cross(f, up));
    vec3 u = cross(s, f);
    return mat4(
        vec4(s, 0.0),
        vec4(u, 0.0),
        vec4(-f, 0.0),
        vec4(0.0, 0.0, 0.0, 1)
    );
}

)" + 1;
	}

	virtual std::string GetBody() const override
	{
		return R"(
view_dir = ray_direction(45.0, #u_resolution#, gl_FragCoord.xy);

eye = vec3(8.0, 5.0, 7.0);

mat4 view2world = view_matrix(eye, vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));

vec3 world_dir = (view2world * vec4(view_dir, 0.0)).xyz;

dist = shortest_distance_to_surface(eye, world_dir, MIN_DIST, MAX_DIST);
if (dist > MAX_DIST - EPSILON_RM) {
    // Didn't hit anything
    #_out# = vec4(0.0, 0.0, 0.0, 0.0);
} else {
	//// no color
	//#_out# = vec4(1.0, 0.0, 0.0, 1.0);

	// The closest point on the surface to the eyepoint along the view ray
	vec3 p = eye + dist * world_dir;

	vec3 K_a = vec3(0.2, 0.2, 0.2);
	vec3 K_d = vec3(0.7, 0.2, 0.2);
	vec3 K_s = vec3(1.0, 1.0, 1.0);
	float shininess = 10.0;

	vec3 color = #lighting#(K_a, K_d, K_s, shininess, p, eye);

	#_out# = vec4(color, 1.0);
}

)" + 1;
	}

	RTTR_ENABLE(Node)

}; // Raymarching

}
}
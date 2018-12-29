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
			{ t_uv,   "uv" },
			{ t_func, "sdf" },
			{ t_func, "lighting" },
		}, {
			{ t_flt4, "_out" },
		}, {
			{ t_flt2, "resolution" },
			{ t_flt3, "dir" },
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
		ID_UV = 0,
		ID_SDF,
		ID_LIGHTING,
	};

protected:
	virtual std::string GetHeader() const override
	{
		return R"(

const int MAX_MARCHING_STEPS = 255;
const float MIN_DIST = 0.0;
const float MAX_DIST = 100.0;
const float EPSILON_RM = 0.0001;

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

vec3 ray_direction(float field_of_view, vec2 size, vec2 frag_coord)
{
    vec2 xy = frag_coord - size / 2.0;
    float z = size.y / tan(radians(field_of_view) / 2.0);
    return normalize(vec3(xy, -z));
}

)" + 1;
	}

	virtual std::string GetBody() const override
	{
		return R"(
resolution = vec2(200, 200);
dir = ray_direction(45.0, resolution, #uv# * resolution);
eye = vec3(0.0, 0.0, 5.0);
dist = shortest_distance_to_surface(eye, dir, MIN_DIST, MAX_DIST);
if (dist > MAX_DIST - EPSILON_RM) {
    // Didn't hit anything
    #_out# = vec4(0.0, 0.0, 0.0, 0.0);
} else {
	//// no color
	//#_out# = vec4(1.0, 0.0, 0.0, 1.0);

	// The closest point on the surface to the eyepoint along the view ray
	vec3 p = eye + dist * dir;

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
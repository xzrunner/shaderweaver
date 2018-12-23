#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

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
		}, {
			{ t_flt4, "_out" },
		}, {
			{ t_flt2, "resolution" },
			{ t_flt3, "dir" },
			{ t_flt3, "eye" },
			{ t_flt1, "dist" },
		});

		auto sdf = std::make_shared<Node>("sdf");
		sdf->InitVariables({
			{ t_flt3, "pos" },
		}, {
			{ t_flt1, "dis" },
		}, {
		});
		AddNesting(sdf);
	}

	enum InputID
	{
		ID_UV = 0,
		ID_SDF,
	};

protected:
	virtual std::string GetHeader() const override
	{
		return R"(

const int MAX_MARCHING_STEPS = 255;
const float MIN_DIST = 0.0;
const float MAX_DIST = 100.0;
const float EPSILON = 0.0001;

float shortest_distance_to_surface(vec3 eye, vec3 marchingDirection, float start, float end)
{
    float depth = start;
    for (int i = 0; i < MAX_MARCHING_STEPS; i++) {
        float dist = #sdf#(eye + depth * marchingDirection);
        if (dist < EPSILON) {
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
if (dist > MAX_DIST - EPSILON) {
    // Didn't hit anything
    #_out# = vec4(0.0, 0.0, 0.0, 0.0);
} else {
	#_out# = vec4(1.0, 0.0, 0.0, 1.0);
}

)" + 1;
	}

	RTTR_ENABLE(Node)

}; // Raymarching

}
}
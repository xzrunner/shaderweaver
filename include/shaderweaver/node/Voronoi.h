#pragma once

#include "shaderweaver/Node.h"

namespace sw
{
namespace node
{

class Voronoi : public sw::Node
{
public:
	Voronoi()
		: Node("Voronoi")
	{
		AddVariable(Variable(t_uv | t_n_in, "uv"));
		AddVariable(Variable(t_flt1 | t_n_in, "angle_offset"));
		AddVariable(Variable(t_flt1 | t_n_in, "cell_density"));

		AddVariable(Variable(t_flt1 | t_n_out, "voronoi"));
		AddVariable(Variable(t_flt1 | t_n_out, "cells"));

		AddVariable(Variable(t_flt2 | t_n_mid, "g"));
		AddVariable(Variable(t_flt2 | t_n_mid, "f"));
		AddVariable(Variable(t_flt1 | t_n_mid, "t"));
		AddVariable(Variable(t_flt3 | t_n_mid, "res"));

	}

	enum ImportIdx
	{
		IN_UV = 0,
		IN_ANGLE_OFFSET,
		IN_CELL_DENSITY,
	};

	enum ExportIdx
	{
		OUT_VORONOI = 0,
		OUT_CELLS,
	};

protected:
	virtual std::string GetHeader() const
	{
		return R"(
vec2 shaderweaver_voronoi_noise_random_vector(vec2 uv, float offset)
{
    mat2 m = mat2(15.27, 47.63, 99.41, 89.98);
    uv = fract(sin(m * uv) * 46839.32);
    return vec2(sin(uv.y*+offset)*0.5+0.5, cos(uv.x*offset)*0.5+0.5);
}
)" + 1;
	}

	virtual std::string GetBody() const override
	{
		return R"(
(g) = floor((uv) * (cell_density));
(f) = fract((uv) * (cell_density));
(t) = 8.0;
(res) = vec3(8.0, 0.0, 0.0);

for(int y=-1; y<=1; y++)
{
    for(int x=-1; x<=1; x++)
    {
        vec2 lattice = vec2(x,y);
        vec2 offset = shaderweaver_voronoi_noise_random_vector(lattice + (g), (angle_offset));
        float d = distance(lattice + offset, (f));
        if(d < (res).x)
        {
            (res) = vec3(d, offset.x, offset.y);
            (voronoi) = (res).x;
            (cells) = (res).y;
         }
    }
}
)" + 1;
	}

}; // Voronoi

}
}
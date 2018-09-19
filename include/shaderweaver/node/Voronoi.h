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
		InitVariables({
			{ t_uv, "uv" },
			{ t_flt1, "angle_offset" },
			{ t_flt1, "cell_density" },
		}, {
			{ t_flt1, "voronoi" },
			{ t_flt1, "cells" },
		}, {
			{ t_flt2, "g" },
			{ t_flt2, "f" },
			{ t_flt1, "t" },
			{ t_flt3, "res" },
		});
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
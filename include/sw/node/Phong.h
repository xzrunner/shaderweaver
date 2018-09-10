#pragma once

#include "sw/Node.h"
#include "sw/VariableType.h"

namespace sw
{
namespace node
{

class Phong : public sw::Node
{
public:
	Phong()
		: Node("Phong")
	{
		// ambient
		AddVariable(Variable(t_flt  | t_n_in, "light_ambient"));
		AddVariable(Variable(t_col3 | t_n_in, "material_diffuse"));
		AddVariable(Variable(t_col3 | t_n_mid, "ambient"));
		// diffuse
		AddVariable(Variable(t_nor3 | t_n_in, "normal"));
		AddVariable(Variable(t_nor3 | t_unit | t_n_mid, "norm"));
		AddVariable(Variable(t_pos3 | t_n_in, "light_pos"));
		AddVariable(Variable(t_flt3 | t_n_in, "frag_pos"));
		AddVariable(Variable(t_nor3 | t_unit | t_n_mid, "light_dir"));
		AddVariable(Variable(t_flt1 | t_n_mid, "diff"));
		AddVariable(Variable(t_flt  | t_n_in, "light_diffuse"));
		AddVariable(Variable(t_col3 | t_n_mid, "diffuse"));
		// specular
		AddVariable(Variable(t_pos3 | t_n_in, "view_pos"));
		AddVariable(Variable(t_nor3 | t_unit | t_n_mid, "view_dir"));
		AddVariable(Variable(t_nor3 | t_unit | t_n_mid, "reflect_dir"));
		AddVariable(Variable(t_flt  | t_n_in, "light_specular"));
		AddVariable(Variable(t_flt1 | t_n_mid, "spec"));
		AddVariable(Variable(t_flt1 | t_n_in, "material_shininess"));
		AddVariable(Variable(t_col3 | t_n_in, "material_specular"));
		AddVariable(Variable(t_col3 | t_n_mid, "specular"));
		// emission
		AddVariable(Variable(t_col3 | t_n_in, "material_emission"));
		AddVariable(Variable(t_col3 | t_n_mid, "emission"));
		// end
		AddVariable(Variable(t_col3 | t_n_out, "phong"));
	}

	enum ImportIdx
	{
		IN_LIGHT_AMBIENT = 0,
		IN_MATERIAL_DIFFUSE,

		IN_NORMAL,
		IN_LIGHT_POSITION,
		IN_FRAG_POS,
		IN_LIGHT_DIFFUSE,

		IN_VIEW_POS,
		IN_LIGHT_SPECULAR,
		IN_MATERIAL_SHININESS,
		IN_MATERIAL_SPECULAR,

		IN_MATERIAL_EMISSION,
	};

protected:
	virtual std::string GetBody() const override
	{
		return R"(
// ambient
(ambient) = (light_ambient) * (material_diffuse);

// diffuse
(norm)      = normalize((normal));
(light_dir) = normalize((light_pos) - (frag_pos));
(diff)      = max(dot((norm), (light_dir)), 0.0);
(diffuse)   = (light_diffuse) * (diff) * (material_diffuse);

// specular
(view_dir)    = normalize((view_pos) - (frag_pos));
(reflect_dir) = reflect(-(light_dir), (norm));
(spec)        = pow(max(dot((view_dir), (reflect_dir)), 0.0), (material_shininess));
(specular)    = (light_specular) * (spec) * (material_specular);

// emission
(emission) = (material_emission);

// end
(phong) = (ambient) + (diffuse) + (specular) + (emission);
)" + 1;
	}

}; // Phong

}
}
#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

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
		InitVariables({
			{ t_flt, "light_ambient" },
			{ t_col3, "material_diffuse" },
			{ t_nor3, "normal" },
			{ t_pos3, "light_pos" },
			{ t_flt3, "frag_pos" },
			{ t_flt, "light_diffuse" },
			{ t_pos3, "view_pos" },
			{ t_flt, "light_specular" },
			{ t_flt1, "material_shininess" },
			{ t_col3, "material_specular" },
			{ t_col3, "material_emission" },
		}, {
			{ t_col3, "phong" },
		}, {
			{ t_col3, "ambient" },
			{ t_nor3 | t_unit, "norm" },
			{ t_nor3 | t_unit, "light_dir" },
			{ t_flt1, "diff" },
			{ t_col3, "diffuse" },
			{ t_nor3 | t_unit, "view_dir" },
			{ t_nor3 | t_unit, "reflect_dir" },
			{ t_flt1, "spec" },
			{ t_col3, "specular" },
			{ t_col3, "emission" },
		});
	}

	enum InputID
	{
		ID_LIGHT_AMBIENT = 0,
		ID_MATERIAL_DIFFUSE,

		ID_NORMAL,
		ID_LIGHT_POSITION,
		ID_FRAG_POS,
		ID_LIGHT_DIFFUSE,

		ID_VIEW_POS,
		ID_LIGHT_SPECULAR,
		ID_MATERIAL_SHININESS,
		ID_MATERIAL_SPECULAR,

		ID_MATERIAL_EMISSION,
	};

protected:
	virtual std::string GetBody() const override
	{
		return R"(
// ambient
#ambient# = #light_ambient# * #material_diffuse#;

// diffuse
#norm#      = normalize(#normal#);
#light_dir# = normalize(#light_pos# - #frag_pos#);
#diff#      = max(dot(#norm#, #light_dir#), 0.0);
#diffuse#   = #light_diffuse# * #diff# * #material_diffuse#;

// specular
#view_dir#    = normalize(#view_pos# - #frag_pos#);
#reflect_dir# = reflect(-#light_dir#, #norm#);
#spec#        = pow(max(dot(#view_dir#, #reflect_dir#), 0.0), #material_shininess#);
#specular#    = #light_specular# * #spec# * #material_specular#;

// emission
#emission# = #material_emission#;

// end
#phong# = #ambient# + #diffuse# + #specular# + #emission#;
)" + 1;
	}

	RTTR_ENABLE(Node)

}; // Phong

}
}
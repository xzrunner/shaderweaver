#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class Combine : public sw::Node
{
public:
	Combine()
		: Node("Combine")
	{
		InitVariables({
			{ t_flt1,  "r" },
			{ t_flt1,  "g" },
			{ t_flt1,  "b" },
			{ t_flt1,  "a" },
		}, {
			{ t_flt4, "rgba" },
			{ t_flt3, "rgb" },
			{ t_flt2, "rg" },
		}, {
		});
	}

	enum InputID
	{
		ID_R = 0,
		ID_G,
		ID_B,
		ID_A,
	};

	enum OutputID
	{
		ID_RGBA = 0,
		ID_RGB,
		ID_RG,
	};

protected:
	virtual std::string GetBody() const override
	{
		std::string ret;
		if (!m_exports[ID_RGBA].conns.empty()) {
			ret += "#rgba# = vec4(#r#, #g#, #b#, #a#);\n";
		}
		if (!m_exports[ID_RGB].conns.empty()) {
			ret += "#rgb# = vec3(#r#, #g#, #b#);\n";
		}
		if (!m_exports[ID_RG].conns.empty()) {
			ret += "#rg# = vec2(#r#, #g#);\n";
		}
		return ret;
	}

	RTTR_ENABLE(Node)

}; // Combine

}
}
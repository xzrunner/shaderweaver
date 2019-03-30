#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class CombineTwo : public sw::Node
{
public:
	CombineTwo()
		: Node("CombineTwo")
	{
		InitVariables({
			{ 0,  "a" },
            { 0,  "b" },
		}, {
			{ t_flt4, "rgba" },
			{ t_flt3, "rgb" },
			{ t_flt2, "rg" },
		}, {
		});
	}

	enum InputID
	{
		ID_A = 0,
		ID_B,
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
			ret += "#rgba# = vec4(#a#, #b#);\n";
		}
		if (!m_exports[ID_RGB].conns.empty()) {
			ret += "#rgb# = vec3(#a#, #b#);\n";
		}
		if (!m_exports[ID_RG].conns.empty()) {
			ret += "#rg# = vec2(#a#, #b#);\n";
		}
		return ret;
	}

	RTTR_ENABLE(Node)

}; // CombineTwo

}
}
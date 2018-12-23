#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class SampleTex2D : public sw::Node
{
public:
	SampleTex2D()
		: Node("SampleTex2D")
	{
		InitVariables({
			{ t_tex2d, "tex" },
			{ t_uv, "uv" },
		}, {
			{ t_col4 | t_read, "rgba" },
			{ t_flt1 | t_read, "r" },
			{ t_flt1 | t_read, "g" },
			{ t_flt1 | t_read, "b" },
			{ t_flt1 | t_read, "a" },
		}, {
		});
	}

	enum InputID
	{
		ID_TEX = 0,
		ID_UV,
	};

	enum OutputID
	{
		ID_RGBA = 0,
		ID_R,
		ID_G,
		ID_B,
		ID_A,
	};

protected:
	virtual std::string GetBody() const override
	{
		std::string ret;

		// todo: in phong, result will be changed by another one before using
		// 1. clip phong to small block, or
		// 2. add special flag to make result read only
		ret += "#rgba# = texture2D(#tex#, #uv#);\n";

		if (!m_exports[ID_R].conns.empty()) {
			ret += "#r# = #rgba#.r;\n";
		}
		if (!m_exports[ID_G].conns.empty()) {
			ret += "#g# = #rgba#.g;\n";
		}
		if (!m_exports[ID_B].conns.empty()) {
			ret += "#b# = #rgba#.b;\n";
		}
		if (!m_exports[ID_A].conns.empty()) {
			ret += "#a# = #rgba#.a;\n";
		}

		return ret;
	}

	RTTR_ENABLE(Node)

}; // SampleTex2D

}
}
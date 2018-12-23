#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class MatrixConstruction : public sw::Node
{
public:
	MatrixConstruction(bool row)
		: Node("MatrixConstruction")
		, m_row(row)
	{
		InitVariables({
			{ t_flt4, "m0" },
			{ t_flt4, "m1" },
			{ t_flt4, "m2" },
			{ t_flt4, "m3" },
		}, {
			{ t_mat4, "mt4" },
			{ t_mat3, "mt3" },
			{ t_mat2, "mt2" },
		}, {
		});
	}

	enum InputID
	{
		ID_M0 = 0,
		ID_M1,
		ID_M2,
		ID_M3,
	};

	enum OutputID
	{
		ID_MAT4,
		ID_MAT3,
		ID_MAT2,
	};

protected:
	virtual std::string GetBody() const override
	{
		std::string ret;
		if (m_row)
		{
			if (!m_exports[ID_MAT4].conns.empty()) {
				ret += "#mt4# = mat4(#m0#.x, #m0#.y, #m0#.z, #m0#.w, #m1#.x, #m1#.y, #m1#.z, #m1#.w, #m2#.x, #m2#.y, #m2#.z, #m2#.w, #m3#.x, #m3#.y, #m3#.z, #m3#.w);\n";
			}
			if (!m_exports[ID_MAT3].conns.empty()) {
				ret += "#mt3# = mat4(#m0#.x, #m0#.y, #m0#.z, #m1#.x, #m1#.y, #m1#.z, #m2#.x, #m2#.y, #m2#.z);\n";
			}
			if (!m_exports[ID_MAT2].conns.empty()) {
				ret += "#mt2# = mat4(#m0#.x, #m0#.y, #m1#.x, #m1#.y);\n";
			}
		}
		else
		{
			if (!m_exports[ID_MAT4].conns.empty()) {
				ret += "#mt4# = mat4(#m0#.x, #m1#.x, #m2#.x, #m3#.x, #m0#.y, #m1#.y, #m2#.y, #m3#.y, #m0#.z, #m1#.z, #m2#.z, #m3#.z, #m0#.w, #m1#.w, #m2#.w, #m3#.w);\n";
			}
			if (!m_exports[ID_MAT3].conns.empty()) {
				ret += "#mt3# = mat4(#m0#.x, #m1#.x, #m2#.x, #m0#.y, #m1#.y, #m2#.y, #m0#.z, #m1#.z, #m2#.z);\n";
			}
			if (!m_exports[ID_MAT2].conns.empty()) {
				ret += "#mt2# = mat4(#m0#.x, #m1#.x, #m0#.y, #m1#.y);\n";
			}
		}
		return ret;
	}

private:
	bool m_row;

	RTTR_ENABLE(Node)

}; // MatrixConstruction

}
}
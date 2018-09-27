#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"

namespace sw
{
namespace node
{

class MatrixSplit : public sw::Node
{
public:
	MatrixSplit(bool row)
		: Node("MatrixSplit")
		, m_row(row)
	{
		InitVariables({
			{ t_d_mat, "_in" },
		}, {
			{ t_d_vec, "m0" },
			{ t_d_vec, "m1" },
			{ t_d_vec, "m2" },
			{ t_d_vec, "m3" },
		}, {
		});

		m_dim_group = { 0, MAX_IMPORTS_COUNT + ID_M0, MAX_IMPORTS_COUNT + ID_M1, MAX_IMPORTS_COUNT + ID_M2, MAX_IMPORTS_COUNT + ID_M3 };
	}

	enum OutputID
	{
		ID_M0 = 0,
		ID_M1,
		ID_M2,
		ID_M3,
	};

protected:
	virtual std::string GetBody() const override
	{
		std::string ret;
		auto dim = GetImports()[0].var.Type().dim;
		bool out[4] = {
			!m_exports[ID_M0].conns.empty(),
			!m_exports[ID_M1].conns.empty(),
			!m_exports[ID_M2].conns.empty(),
			!m_exports[ID_M3].conns.empty(),
		};
		if (m_row)
		{
			switch (dim)
			{
			case 4:
				if (out[0]) { ret += "#m0# = vec4(#_in#[0][0], #_in#[0][1], #_in#[0][2], #_in#[0][3]);\n"; }
				if (out[1]) { ret += "#m1# = vec4(#_in#[1][0], #_in#[1][1], #_in#[1][2], #_in#[1][3]);\n"; }
				if (out[2]) { ret += "#m2# = vec4(#_in#[2][0], #_in#[2][1], #_in#[2][2], #_in#[2][3]);\n"; }
				if (out[3]) { ret += "#m3# = vec4(#_in#[3][0], #_in#[3][1], #_in#[3][2], #_in#[3][3]);\n"; }
				break;
			case 3:
				if (out[0]) { ret += "#m0# = vec3(#_in#[0][0], #_in#[0][1], #_in#[0][2]);\n"; }
				if (out[1]) { ret += "#m1# = vec3(#_in#[1][0], #_in#[1][1], #_in#[1][2]);\n"; }
				if (out[2]) { ret += "#m2# = vec3(#_in#[2][0], #_in#[2][1], #_in#[2][2]);\n"; }
				break;
			case 2:
				if (out[0]) { ret += "#m0# = vec2(#_in#[0][0], #_in#[0][1]);\n"; }
				if (out[1]) { ret += "#m1# = vec2(#_in#[1][0], #_in#[1][1]);\n"; }
				break;
			}
		}
		else
		{
			switch (dim)
			{
			case 4:
				if (out[0]) { ret += "#m0# = vec4(#_in#[0][0], #_in#[1][0], #_in#[2][0], #_in#[3][0]);\n"; }
				if (out[1]) { ret += "#m1# = vec4(#_in#[0][1], #_in#[1][1], #_in#[2][1], #_in#[3][1]);\n"; }
				if (out[2]) { ret += "#m2# = vec4(#_in#[0][2], #_in#[1][2], #_in#[2][2], #_in#[3][2]);\n"; }
				if (out[3]) { ret += "#m3# = vec4(#_in#[0][3], #_in#[1][3], #_in#[2][3], #_in#[3][3]);\n"; }
				break;
			case 3:
				if (out[0]) { ret += "#m0# = vec4(#_in#[0][0], #_in#[1][0], #_in#[2][0]);\n"; }
				if (out[1]) { ret += "#m1# = vec4(#_in#[0][1], #_in#[1][1], #_in#[2][1]);\n"; }
				if (out[2]) { ret += "#m2# = vec4(#_in#[0][2], #_in#[1][2], #_in#[2][2]);\n"; }
				break;
			case 2:
				if (out[0]) { ret += "#m0# = vec4(#_in#[0][0], #_in#[1][0]);\n"; }
				if (out[1]) { ret += "#m1# = vec4(#_in#[0][1], #_in#[1][1]);\n"; }
				break;
			}
		}
		return ret;
	}

private:
	bool m_row;

}; // MatrixSplit

}
}